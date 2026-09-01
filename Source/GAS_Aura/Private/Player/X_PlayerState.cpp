// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))


#include "Player/X_PlayerState.h"
#include "AbilitySystem/X_AbilitySystemComponent.h"
#include "AbilitySystem/X_AttributeSet.h"

#pragma region Unreal Defaults

AX_PlayerState::AX_PlayerState()
{
	InitComponentsForAbilitySystem();
}

#pragma endregion Unreal Defaults

#pragma region Ability System

UAbilitySystemComponent* AX_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AX_PlayerState::InitComponentsForAbilitySystem()
{
	// A value of 100.0f (100 updates per second) is critical for responsiveness in a multiplayer environment, especially when using the Gameplay Ability System (GAS).
	// Set to 100.0 so that HP bars, cooldowns, and status effects update instantly on the client screen, matching the fast-paced feel of the combat.
	// Without it, the UI would feel "laggy" even if the Player's ping is low.
	SetNetUpdateFrequency(100.0);

	// Create the AbilitySystemComponent using our custom X_AbilitySystemComponent class.
	AbilitySystemComponent = CreateDefaultSubobject<UX_AbilitySystemComponent>("AbilitySystemComponent");

	// Make sure it is replicated for multiplayer.
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed is the best option for Players.  The Server replicates all GameplayEffects to the owning client only.  Gameplay Cues and Gameplay Tags are replicated to all clients.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the AttributeSet using our custom X_AttributeSet class.
	AttributeSet = CreateDefaultSubobject<UX_AttributeSet>("AttributeSet");
}

#pragma endregion Ability System
