// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#include "Characters/X_Character_NPC.h"

#include "AbilitySystem/X_AbilitySystemComponent.h"
#include "AbilitySystem/X_AttributeSet.h"

#pragma region Unreal Defaults

AX_Character_NPC::AX_Character_NPC()
{
	// Suppress tick queue overhead until active AI behavior ticking is required
	PrimaryActorTick.bCanEverTick = false;
	
	InitComponentsForAbilitySystem();
}

void AX_Character_NPC::BeginPlay()
{
	Super::BeginPlay();
	
}

#pragma endregion Unreal Defaults

#pragma region Ability System

UAbilitySystemComponent* AX_Character_NPC::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AX_Character_NPC::GetAttributeSet() const
{
	return AttributeSet;
}

void AX_Character_NPC::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// CLIENT-SIDE ONLY: Initialize the ASC early.
	if (!HasAuthority())
	{
		InitAbilitySystemServerAndClientSide();
	}
	
}

void AX_Character_NPC::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// SERVER-SIDE ONLY: 
	if (HasAuthority())
	{
		// Initialize the ASC now that the AI Controller is officially set.
		InitAbilitySystemServerAndClientSide();
		
		// Initialize the AI Behavior.
		//InitAIBehavior(NewController);
	}
}

void AX_Character_NPC::InitComponentsForAbilitySystem()
{
	// Create the AbilitySystemComponent using our custom X_AbilitySystemComponent class.
	AbilitySystemComponent = CreateDefaultSubobject<UX_AbilitySystemComponent>("AbilitySystemComponent");

	// Make sure it is replicated for multiplayer.
	AbilitySystemComponent->SetIsReplicated(true);

	// Minimal is the best option for NPCs.  Gameplay Effects are NOT replicated since NPCs run on the server (there's no need for replication).  Gameplay Cues and Gameplay Tags are replicated to all clients.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// Create the AttributeSet using our custom X_AttributeSet class.
	AttributeSet = CreateDefaultSubobject<UX_AttributeSet>("AttributeSet");
}

void AX_Character_NPC::InitAbilitySystemServerAndClientSide()
{
	// We need the custom version to call BindToGameplayEffectDelegates, so if we can't get our custom ASC, there's no point in continuing.
	UX_AbilitySystemComponent* XASC = Cast<UX_AbilitySystemComponent>(GetAbilitySystemComponent());
	if (!ensureMsgf(IsValid(XASC), TEXT("Actor: %s - No valid (XASC) found.  Function: %hs"),
				   *GetName(), __FUNCTION__))
	{
		return;
	}
	
	// This is simply a sanity check to prevent execution of InitAbilityActorInfo if we can't get our custom AS to prevent headaches down the line.
	UX_AttributeSet* XAS = Cast<UX_AttributeSet>(GetAttributeSet());
	if (!ensureMsgf(IsValid(XAS), TEXT("Actor: %s - No valid (XAS) found.  Function: %hs"),
				   *GetName(), __FUNCTION__))
	{
		return;
	}

	// Initialize the Ability System (OwnerActor = This, AvatarActor = This)
	// This communicates to the XASC who its OwnerActor and AvatarActor are.
	XASC->InitAbilityActorInfo(this, this);

	// Bind to the ToTarget and ToSelf custom delegates.
	//XASC->BindToGameplayEffectDelegates();
	
	// This ensures the UI is listening before the Attributes are given their starting values.
	//BindToAttributeDelegates();
	
	// Called here to ensure no tags are missed when applied on Character spawn.
	//BindToGameplayTagEventDelegates();

	// Server-Side Only: Initialize Attributes and Grant Startup Abilities
	if (HasAuthority() && !bAbilitySystemInitialized)
	{
		/*
		InitializeAttributes(this, this, this, GetCharacterLevel());
		InitializeDefaultGameplayTags(this, this, this, GetCharacterLevel());
		GrantClassDefaultGameplayAbilitiesOnStartup(GetCharacterLevel());
		GrantClassSharedGameplayAbilitiesOnStartup(GetCharacterLevel());
		*/
		bAbilitySystemInitialized = true;
	}	
}

#pragma endregion Ability System