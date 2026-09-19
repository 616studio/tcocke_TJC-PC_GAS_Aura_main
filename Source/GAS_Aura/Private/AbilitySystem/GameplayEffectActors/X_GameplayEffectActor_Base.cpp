// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))


#include "AbilitySystem/GameplayEffectActors/X_GameplayEffectActor_Base.h"
#include "AbilitySystem/X_AttributeSet.h"


AX_GameplayEffectActor_Base::AX_GameplayEffectActor_Base()
{
	PrimaryActorTick.bCanEverTick = false;

	// We want components and meshes to be added in the Blueprint, so we create a simple Scene Component here that will act as the RootComponent.
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

	bReplicates = true;
}

void AX_GameplayEffectActor_Base::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void AX_GameplayEffectActor_Base::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// Preserves native engine delegates and allows child Blueprints to optionally listen to ActorBeginOverlap.
	Super::NotifyActorBeginOverlap(OtherActor);

	// Authoritative server guard for GAS effect application.
	// We do this because modifying Gameplay Ability System states (Attributes and Gameplay Effects) must be strictly server-authoritative to prevent cheat vulnerabilities, double-application, and replication desyncs.
	if (HasAuthority())
	{
		OnBeginOverlap(OtherActor);
	}
}

void AX_GameplayEffectActor_Base::NotifyActorEndOverlap(AActor* OtherActor)
{
	// Preserves native engine delegates and allows child Blueprints to optionally listen to ActorEndOverlap.
	Super::NotifyActorEndOverlap(OtherActor);

	// Authoritative server guard for GAS effect application.
	// We do this because modifying Gameplay Ability System states (Attributes and Gameplay Effects) must be strictly server-authoritative to prevent cheat vulnerabilities, double-application, and replication desyncs.
	if (HasAuthority())
	{
		OnEndOverlap(OtherActor);
	}
}

bool AX_GameplayEffectActor_Base::ApplyGameplayEffectToTarget(AActor* TargetActor,
	TSubclassOf<UGameplayEffect> GameplayEffectClassToApply)
{
	if (!IsValid(GameplayEffectClassToApply)) return false;
	
	UAbilitySystemComponent* TargetActorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (!IsValid(TargetActorASC)) return false;

	// GAS requires GE's to be wrapped first in a ContextHandle so it can be polymorphic and properly replicated.
	// A Context Handle tells the system (for example) who caused the damage, what weapon was used, where the hit happened, and much more.	 
	FGameplayEffectContextHandle EffectContextHandle = TargetActorASC->MakeEffectContext();
	
	// We add this Actor as the Instigator (the Actor who owns the effect being applied) and the Causer (the Actor applying the effect).
	// Since this class was built solely to apply Gameplay Effects that it owns, it will be both the Instigator and EffectCauser.
	EffectContextHandle.AddInstigator(GetInstigator(), this);
	
	// Explicitly set SourceObject to be this class so MMCs, ExecCalcs, and Gameplay Cues can query 'this'.
	EffectContextHandle.AddSourceObject(this);

	// GAS requires GE's to be wrapped a second time in a SpecHandle so it can be properly referenced when instantiated (especially when applied multiple times to multiple targets).	
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetActorASC->MakeOutgoingSpec(GameplayEffectClassToApply, GameplayEffectLevel, EffectContextHandle);

	if (!EffectSpecHandle.IsValid()) return false;
	
	// Apply the Spec.
	// We hand the fully baked math back to the Target's ASC to apply to itself.
	// It returns an ActiveEffectHandle, which acts like a receipt for the transaction.
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetActorASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	// Store the Receipt (For Infinite Effects Only).
	// If the effect is Infinite AND it should be removed OnEndOverlap, add it to our list for tracking removals.
	const FGameplayEffectSpec* EffectSpec = EffectSpecHandle.Data.Get();
	if (EffectSpec && IsValid(EffectSpec->Def))
	{
		if (EffectSpec->Def->DurationPolicy == EGameplayEffectDurationType::Infinite &&
			InfiniteGameplayEffectRemovalPolicy == EX_GameplayEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			ActiveGameplayEffectHandles.Add(ActiveEffectHandle, TargetActorASC);
		}
	}

	return true;
}

void AX_GameplayEffectActor_Base::OnBeginOverlap(AActor* TargetActor)
{
	// SERVER ONLY: Gameplay Effects MUST be applied on the Server. The Server will replicate them down to the clients.
	if (!HasAuthority()) return;
	
	// Check policies and apply effects. 
	// If any effect successfully applies, flag bEffectApplied as true.
	bool bEffectApplied = false;
	
	if (IsValid(InstantGameplayEffectClass))
	{
		if (InstantGameplayEffectApplicationPolicy == EGameplayEffectApplicationPolicy::ApplyOnBeginOverlap)
		{
			if (ApplyGameplayEffectToTarget(TargetActor, InstantGameplayEffectClass))
			{
				bEffectApplied = true;
			}
		}
	}

	if (IsValid(DurationGameplayEffectClass))
	{
		if (DurationGameplayEffectApplicationPolicy == EGameplayEffectApplicationPolicy::ApplyOnBeginOverlap)
		{
			if (ApplyGameplayEffectToTarget(TargetActor, DurationGameplayEffectClass))
			{
				bEffectApplied = true;
			}
		}
	}

	if (IsValid(InfiniteGameplayEffectClass))
	{
		if (InfiniteGameplayEffectApplicationPolicy == EGameplayEffectApplicationPolicy::ApplyOnBeginOverlap)
		{
			if (ApplyGameplayEffectToTarget(TargetActor, InfiniteGameplayEffectClass))
			{
				bEffectApplied = true;
			}
		}
	}

	// If an effect was applied and the Actor is set to destroy itself, then destroy.
	if (bEffectApplied && bDestroyOnEffectApplication)
	{
		Destroy();
	}
}

void AX_GameplayEffectActor_Base::OnEndOverlap(AActor* TargetActor)
{
	// SERVER ONLY
	if (!HasAuthority()) return;
	
	// Same pattern as BeginOverlap, just checking the EndOverlap policy instead.
	
	if (IsValid(InstantGameplayEffectClass))
	{
		if (InstantGameplayEffectApplicationPolicy == EGameplayEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyGameplayEffectToTarget(TargetActor, InstantGameplayEffectClass);
		}
	}

	if (IsValid(DurationGameplayEffectClass))
	{
		if (DurationGameplayEffectApplicationPolicy == EGameplayEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyGameplayEffectToTarget(TargetActor, DurationGameplayEffectClass);
		}
	}

	if (IsValid(InfiniteGameplayEffectClass))
	{
		if (InfiniteGameplayEffectApplicationPolicy == EGameplayEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyGameplayEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
		}
	}

	// --- REMOVING INFINITE EFFECTS ---
	// If there are no active infinite effects stored, we can leave early.
	if (ActiveGameplayEffectHandles.IsEmpty()) return;

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (!IsValid(TargetASC)) return;

	// We can't remove items from a Map while we are actively looping through it (it causes a crash).
	// So, we create a temporary array to store the receipts we plan to throw away.
	TArray<FActiveGameplayEffectHandle> EffectHandlesToRemove;
	
	// Now we look through our TMap and compare the TargetActor's ASC.
	for (const auto& HandlePair : ActiveGameplayEffectHandles)
	{
		// If the ASC stored in our receipt book matches the ASC of the Actor who just left the overlapping component of this Actor...
		if (TargetASC == HandlePair.Value.Get())
		{
			// Force their ASC to cancel the effect using the receipt (HandlePair.Key)
			TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, InfiniteEffectRemovalStacks);
			
			// Add the receipt to our temporary array.
			EffectHandlesToRemove.Add(HandlePair.Key);
		}
	}

	// Now that we are done looping, we can safely empty our temporary array out of the main TMap.
	for (const auto& EffectHandle : EffectHandlesToRemove)
	{
		// Safe removal. If it's not there, it does nothing. 
		ActiveGameplayEffectHandles.Remove(EffectHandle);
	}

	// Ensure no other actors are still being affected before destroying this Actor.
	// If this Actor is supposed to destroy itself once everyone leaves (like a temporary smoke cloud)...
	// AND we actually removed effects this frame...
	// AND the receipt book is completely empty (meaning nobody else is standing inside)...
	if (bDestroyOnEffectRemoval && EffectHandlesToRemove.Num() > 0 && ActiveGameplayEffectHandles.IsEmpty())
	{
		Destroy();
	}
}