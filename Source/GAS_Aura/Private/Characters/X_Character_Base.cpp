// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#include "GAS_Aura/Public/Characters/X_Character_Base.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/X_AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/X_GameplayAbility_Base.h"
#include "DataAssets/X_CharacterClassInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameState/X_GameState_Base.h"
#include "Kismet/GameplayStatics.h"

#pragma region Unreal Defaults

AX_Character_Base::AX_Character_Base()
{
	// No reason for this base class to tick.
	PrimaryActorTick.bCanEverTick = false;
	
	InitWeapon();
}

void AX_Character_Base::BeginPlay()
{
	Super::BeginPlay();
	
	InitCharacterBehavior();
	
}

#pragma endregion Unreal Defaults

#pragma region Ability System

UAbilitySystemComponent* AX_Character_Base::GetAbilitySystemComponent() const
{
	return nullptr;
}

UAttributeSet* AX_Character_Base::GetAttributeSet() const
{
	return nullptr;
}

void AX_Character_Base::SetCharacterLevel(const int32 NewLevel)
{
}

void AX_Character_Base::SetCharacterClass(const ECharacterClass NewClassType)
{
}

void AX_Character_Base::InitializeAttributes(UObject* InSourceObject, AActor* InInstigator, AActor* InEffectCauser, const ECharacterClass InCharacterClass, const int32 InCharacterLevel)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ensureMsgf(IsValid(ASC), TEXT("Actor: %s - No valid (ASC) found.  Function: %hs"),
				   *GetName(), 
				   __FUNCTION__))
	{
		return;
	}
	
	// Retrieve CharacterClassInfo from GameState.
	// Stored on GameState rather than GameMode so client calculations can share the same Data Asset as the server.
	const AX_GameState_Base* GameState = Cast<AX_GameState_Base>(UGameplayStatics::GetGameState(this));
	if (!ensureMsgf(IsValid(GameState), TEXT("Actor: %s - No valid (GameState) found.  Function: %hs"),
	               *GetName(), __FUNCTION__))
	{
		return;
	}
	
	UX_CharacterClassInfo* ClassInfo = GameState->CharacterClassInfo;
	if (!ensureMsgf(IsValid(ClassInfo), TEXT("Actor: %s - Missing data for Editor assigned variable (%s).  Function: %hs"),
				   *GameState->GetName(),
				   *GET_MEMBER_NAME_CHECKED(AX_GameState_Base, CharacterClassInfo).ToString(),
				   __FUNCTION__))
	{
		return;
	}
	
	const FX_CharacterClassDefaultInfo* ClassDefaultInfo = ClassInfo->GetCharacterClassDefaultInfo(InCharacterClass);
	if (!ensureMsgf(ClassDefaultInfo, TEXT("Actor: %s - Missing CharacterClassInfo entry for CharacterClass: %s. Function: %hs"),
				*GetName(),
				*UEnum::GetValueAsString(InCharacterClass),
				__FUNCTION__))
	{
		return;
	}

	const TSubclassOf<UGameplayEffect> PrimaryAttributes = ClassDefaultInfo->PrimaryAttributes;
	if (!ensureMsgf(PrimaryAttributes, TEXT("Actor: %s - Missing data for Editor assigned variable (PrimaryAttributes) for CharacterClass:  %s.  Function: %hs"),
	               *GetName(),
	               *UEnum::GetValueAsString(InCharacterClass),
	               __FUNCTION__))
	{
		return;	
	}

	const TSubclassOf<UGameplayEffect> SecondaryAttributes = ClassDefaultInfo->SecondaryAttributes;
	if (!ensureMsgf(SecondaryAttributes, TEXT("Actor: %s - Missing data for Editor assigned variable (SecondaryAttributes) for CharacterClass:  %s.  Function: %hs"),
				   *GetName(),
				   *UEnum::GetValueAsString(InCharacterClass),
				   __FUNCTION__))
	{
		return;	
	}

	const TSubclassOf<UGameplayEffect> VitalAttributes = ClassDefaultInfo->VitalAttributes;
	if (!ensureMsgf(VitalAttributes, TEXT("Actor: %s - Missing data for Editor assigned variable (VitalAttributes) for CharacterClass:  %s.  Function: %hs"),
				   *GetName(),
				   *UEnum::GetValueAsString(InCharacterClass),
				   __FUNCTION__))
	{
		return;	
	}

	const TSubclassOf<UGameplayEffect> ResistanceAttributes = ClassDefaultInfo->ResistanceAttributes;
	if (!ensureMsgf(ResistanceAttributes, TEXT("Actor: %s - Missing data for Editor assigned variable (ResistanceAttributes) for CharacterClass:  %s.  Function: %hs"),
				   *GetName(),
				   *UEnum::GetValueAsString(InCharacterClass),
				   __FUNCTION__))
	{
		return;	
	}

	// WARNING: EXECUTION ORDER IS CRITICAL.
	// GAS evaluates Gameplay Effects by taking a "snapshot" of Attributes at the exact moment they are applied to the owning ASC.
	// We must guarantee that Attributes which CREATE data are applied before Attributes which NEED that data.
	// 1. Primary Attributes: Establishes base values (each Attribute is set manually with a Modifier Op of "Override" and Magnitude Calc Type of "Scalable Float" with a value of "10").
	// 2. Secondary Attributes: Uses values from Primary Attributes to calculate their own values (X_MMC_MaxHealth calculates MaxHealth using Vigor; X_MMC_MaxMana calculates MaxMana using Intelligence).
	// 3. Vital Attributes: Uses values from Secondary Attributes to calculate their own values (Health copies in the value of MaxHealth; Mana copies in the value of MaxMana).
	// 4. Resistance Attributes:  Uses values from both Primary Attributes and Secondary Attributes.
	ApplyGameplayEffectToSelf(ASC, PrimaryAttributes, InSourceObject, InInstigator, InEffectCauser, InCharacterLevel);
	ApplyGameplayEffectToSelf(ASC, SecondaryAttributes, InSourceObject, InInstigator, InEffectCauser, InCharacterLevel);
	ApplyGameplayEffectToSelf(ASC, VitalAttributes, InSourceObject, InInstigator, InEffectCauser, InCharacterLevel);
	ApplyGameplayEffectToSelf(ASC, ResistanceAttributes, InSourceObject, InInstigator, InEffectCauser, InCharacterLevel);
}

void AX_Character_Base::InitializeDefaultGameplayTags(UObject* InSourceObject, AActor* InInstigator,
	AActor* InEffectCauser, const ECharacterClass InCharacterClass, const int32 InCharacterLevel)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ensureMsgf(IsValid(ASC), TEXT("Actor: %s - No valid (ASC) found.  Function: %hs"),
				   *GetName(), 
				   __FUNCTION__))
	{
		return;
	}
	
	// Retrieve CharacterClassInfo from GameState.
	// Stored on GameState rather than GameMode so client calculations can share the same Data Asset as the server.
	const AX_GameState_Base* GameState = Cast<AX_GameState_Base>(UGameplayStatics::GetGameState(this));
	if (!ensureMsgf(IsValid(GameState), TEXT("Actor: %s - No valid (GameState) found.  Function: %hs"),
	               *GetName(), __FUNCTION__))
	{
		return;
	}
	
	UX_CharacterClassInfo* ClassInfo = GameState->CharacterClassInfo;
	if (!ensureMsgf(IsValid(ClassInfo), TEXT("Actor: %s - Missing data for Editor assigned variable (%s).  Function: %hs"),
				   *GameState->GetName(),
				   *GET_MEMBER_NAME_CHECKED(AX_GameState_Base, CharacterClassInfo).ToString(),
				   __FUNCTION__))
	{
		return;
	}
	
	const FX_CharacterClassDefaultInfo* ClassDefaultInfo = ClassInfo->GetCharacterClassDefaultInfo(InCharacterClass);
	if (!ensureMsgf(ClassDefaultInfo, TEXT("Actor: %s - Missing CharacterClassInfo entry for CharacterClass: %s. Function: %hs"),
				*GetName(),
				*UEnum::GetValueAsString(InCharacterClass),
				__FUNCTION__))
	{
		return;
	}

	const TSubclassOf<UGameplayEffect> DefaultGameplayTags = ClassDefaultInfo->DefaultGameplayTags;
	if (!ensureMsgf(DefaultGameplayTags, TEXT("Actor: %s - Missing data for Editor assigned variable (DefaultGameplayTags) for Character Class:  %s.  Function: %hs"),
				   *GetName(),
				   *UEnum::GetValueAsString(InCharacterClass),
				   __FUNCTION__))
	{
		return;	
	}
	
	ApplyGameplayEffectToSelf(ASC, DefaultGameplayTags, InSourceObject, InInstigator, InEffectCauser, InCharacterLevel);
}

void AX_Character_Base::GrantClassDefaultGameplayAbilitiesOnStartup(const ECharacterClass InCharacterClass, const int32 InCharacterLevel)
{
	// We only want to grant Gameplay Abilities on the Server Side.
	if (!HasAuthority()) return;
	
	// We cast to our custom ASC to ensure we have access to the input functions we use with Gameplay Tags to tie Gameplay Abilities and Input Actions together.
	// Currently, only the Player needs access to this, but it saves having to do conditional casts for each Character type in the future.
	UX_AbilitySystemComponent* XASC = Cast<UX_AbilitySystemComponent>(GetAbilitySystemComponent());
	if (!ensureMsgf(IsValid(XASC), TEXT("Actor: %s - No valid (XASC) found.  Function: %hs"),
				   *GetName(), __FUNCTION__))
	{
		return;
	}
	
	// Retrieve CharacterClassInfo from GameState.
	// Stored on GameState rather than GameMode so client calculations can share the same Data Asset as the server.
	AX_GameState_Base* GameState = Cast<AX_GameState_Base>(UGameplayStatics::GetGameState(this));
	if (!ensureMsgf(IsValid(GameState), TEXT("Actor: %s - No valid (GameState) found.  Function: %hs"),
	               *GetName(), __FUNCTION__))
	{
		return;
	}
	
	UX_CharacterClassInfo* ClassInfo = GameState->CharacterClassInfo;
	if (!ensureMsgf(IsValid(ClassInfo), TEXT("Actor: %s - Missing data for Editor assigned variable (%s).  Function: %hs"),
				   *GameState->GetName(),
				   *GET_MEMBER_NAME_CHECKED(AX_GameState_Base, CharacterClassInfo).ToString(),
				   __FUNCTION__))
	{
		return;
	}
	
	const FX_CharacterClassDefaultInfo* ClassDefaultInfo = ClassInfo->GetCharacterClassDefaultInfo(InCharacterClass);
	if (!ensureMsgf(ClassDefaultInfo, TEXT("Actor: %s - Missing CharacterClassInfo entry for CharacterClass: %s. Function: %hs"),
				*GetName(),
				*UEnum::GetValueAsString(InCharacterClass),
				__FUNCTION__))
	{
		return;
	}
	
	if (!ClassDefaultInfo->bShouldHaveDefaultAbilities) return;

	if (!ensureMsgf(!ClassDefaultInfo->DefaultAbilities.IsEmpty(), TEXT("Actor: %s - %s has no abilities assigned.  Function: %hs"),
	               *GetName(),
	               *UEnum::GetValueAsString(InCharacterClass),
	               __FUNCTION__))
	{
		return;
	}

	for (int32 i = 0; i < ClassDefaultInfo->DefaultAbilities.Num(); ++i)
	{
		const TSubclassOf<UGameplayAbility> StartupAbility = ClassDefaultInfo->DefaultAbilities[i];

		// Check if an empty array element was accidentally added.
		if (!StartupAbility)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor: %s - Found an empty ability entry in %s at Index [%d]. Function: %hs"),
				   *GetName(),
				   *UEnum::GetValueAsString(InCharacterClass),
				   i,
				   __FUNCTION__);
       
			continue;
		}

		// GAS requires us to create an FGameplayAbilitySpec wrapper for Gameplay Abilities before granting them to an ASC.
		// We pass 'this' as the SourceObject since these are default Gameplay Abilities and Gameplay Tags being granted at startup by this Character to this Character.
		FGameplayAbilitySpec StartupAbilitySpec = FGameplayAbilitySpec(StartupAbility, InCharacterLevel, INDEX_NONE, this);

		// We have to cast to our custom X_GameplayAbility_Base class here in order to access our custom Gameplay Tag variable.
		if (const UX_GameplayAbility_Base* CustomStartupAbilityClass = Cast<UX_GameplayAbility_Base>(StartupAbilitySpec.Ability))
		{
			// Add the custom input tag we have assigned to each Gameplay Ability to the StartupAbilitySpec (if it exists).
			if (CustomStartupAbilityClass->GameplayAbilityInputActionTag.IsValid())
			{
				StartupAbilitySpec.GetDynamicSpecSourceTags().AddTag(CustomStartupAbilityClass->GameplayAbilityInputActionTag);	
			}			
		}		
		
		// Grant the StartupAbilitySpec to our custom ASC.
		XASC->GiveAbility(StartupAbilitySpec);
	}
}

void AX_Character_Base::GrantClassSharedGameplayAbilitiesOnStartup(const int32 InCharacterLevel)
{
	// We only want to grant Gameplay Abilities on the Server Side.
	if (!HasAuthority()) return;
	
	// Retrieve CharacterClassInfo from GameState.
	// Stored on GameState rather than GameMode so client calculations can share the same Data Asset as the server.
	AX_GameState_Base* GameState = Cast<AX_GameState_Base>(UGameplayStatics::GetGameState(this));
	if (!GameState) return;
	
	UX_CharacterClassInfo* ClassInfo = GameState->CharacterClassInfo;
	if (!ensureMsgf(IsValid(ClassInfo), TEXT("Actor: %s - Missing data for Editor assigned variable (%s).  Function: %hs"),
				   *GameState->GetName(),
				   *GET_MEMBER_NAME_CHECKED(AX_GameState_Base, CharacterClassInfo).ToString(),
				   __FUNCTION__))
	{
		return;
	}
	
	// We cast to our custom ASC to ensure we have access to the input functions we use with Gameplay Tags to tie Gameplay Abilities and Input Actions together.
	// Currently, only the Player needs access to this, but it saves having to do conditional casts for each Character type.
	UX_AbilitySystemComponent* XASC = Cast<UX_AbilitySystemComponent>(GetAbilitySystemComponent());
	if (!ensureMsgf(IsValid(XASC), TEXT("Actor: %s - No valid (XASC) found.  Function: %hs"),
				   *GetName(), __FUNCTION__))
	{
		return;
	}
		
	if (ClassInfo->SharedGameplayAbilities.IsEmpty()) return;
	
	for (int32 i = 0; i < ClassInfo->SharedGameplayAbilities.Num(); ++i)
	{
		TSubclassOf<UGameplayAbility> SharedAbility = ClassInfo->SharedGameplayAbilities[i];
		
		// Check if an empty SharedGameplayAbilities array element was accidentally added.
		if (!SharedAbility)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor: %s - Found an empty (Shared Gameplay Abilities) entry in (DA_CharacterClassInfo) at Index [%d]. Function: %hs"),
				   *GetName(),
				   i,
				   __FUNCTION__);
       
			continue;
		}
		
		// GAS requires us to create an FGameplayAbilitySpec wrapper for Gameplay Abilities before granting them to an ASC.
		// We pass 'this' as the SourceObject since these are default Gameplay Abilities and Gameplay Tags being granted at startup by this Character to this Character.
		FGameplayAbilitySpec SharedAbilitySpec = FGameplayAbilitySpec(SharedAbility, InCharacterLevel, INDEX_NONE, this);

		// We have to cast to our custom X_GameplayAbility_Base class here in order to access our custom Gameplay Tag variable.
		if (const UX_GameplayAbility_Base* CustomSharedAbilityClass = Cast<UX_GameplayAbility_Base>(SharedAbilitySpec.Ability))
		{
			// Add the custom input tag we have assigned to each Gameplay Ability to the SharedAbilitySpec (if it exists).
			if (CustomSharedAbilityClass->GameplayAbilityInputActionTag.IsValid())
			{
				SharedAbilitySpec.GetDynamicSpecSourceTags().AddTag(CustomSharedAbilityClass->GameplayAbilityInputActionTag);	
			}			
		}	
		
		// Grant the SharedAbilitySpec to our custom ASC.
		XASC->GiveAbility(SharedAbilitySpec);
	}
}

void AX_Character_Base::ApplyGameplayEffectToSelf(UAbilitySystemComponent* ASC, const TSubclassOf<UGameplayEffect> GameplayEffect, const UObject* SourceObject, AActor* InInstigator, AActor* InEffectCauser, const int32 InCharacterLevel) const
{
	if (!ensureMsgf(IsValid(ASC), TEXT("Actor: %s - No valid (ASC) found.  Function: %hs"),
	               *GetName(), __FUNCTION__))
	{
		return;
	}

	if (!ensureMsgf(IsValid(GameplayEffect), TEXT("Actor: %s - No valid (GameplayEffect) found.  Function: %hs"),
	               *GetName(), __FUNCTION__))
	{
		return;
	}
	
	// GAS requires GE's to be wrapped first in a ContextHandle so it can be polymorphic and properly replicated. 
	// A Context Handle tells the system (for example) who caused the damage, what weapon was used, where the hit happened, etc.
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceObject);
	EffectContextHandle.AddInstigator(InInstigator, InEffectCauser);

	// GAS requires GE's to be wrapped a second time in a SpecHandle so it can be properly referenced when instantiated (especially when applied multiple times to multiple targets).
	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GameplayEffect, InCharacterLevel, EffectContextHandle);

	// If for whatever reason the SpecHandle is not valid, we don't want to continue; log an error to the designer and leave.
	if (!ensureMsgf(EffectSpecHandle.IsValid(), TEXT("Actor: %s - No valid (SpecHandle) found.  Function: %hs"), *GetName(), __FUNCTION__))
	{
		return;
	}

	// We use ToTarget and pass our own ASC here. 
	// While ApplyGameplayEffectSpecToSelf exists, it is simply a convenience wrapper that calls ApplyGameplayEffectSpecToTarget under the hood. 
	// Using ToTarget directly skips the wrapper and calls the core networking and math function directly.
	ASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), ASC);
}

#pragma endregion Ability System

#pragma region Weapon

void AX_Character_Base::InitWeapon()
{
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), WeaponHandSocket);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

#pragma endregion Weapon

#pragma region Cursor Highlight Behavior

void AX_Character_Base::AddHighlightToActor()
{
	if (GetMesh())
	{
		GetMesh()->SetRenderCustomDepth(true);
		GetMesh()->SetCustomDepthStencilValue(GetCustomStencilDepthValue());	
	}
	
	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(true);
		Weapon->SetCustomDepthStencilValue(GetCustomStencilDepthValue());
	}
}

void AX_Character_Base::RemoveHighlightFromActor()
{
	if (GetMesh())
	{
		GetMesh()->SetRenderCustomDepth(false);
	}
	
	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(false);
	}
}

float AX_Character_Base::GetCustomStencilDepthValue() const
{
	return 250.0f;
}

#pragma endregion Cursor Highlight Behavior

#pragma region Character Defaults

void AX_Character_Base::InitCharacterBehavior()
{
	// Inject our designer-friendly exposed variable into the engine's actual physics system.
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->Mass = CharacterMass;
	}
}

#pragma endregion Character Defaults