// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))


#include "AbilitySystem/X_AbilitySystemComponent.h"

#include "GAS_Aura/UtilityClasses/X_GameplayTags.h"


void UX_AbilitySystemComponent::BindToGameplayEffectDelegate()
{
	if (bDelegatesBound) return;
	
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::CheckGameplayEffectHasMatchingAssetTags);
	
	bDelegatesBound = true;
}

void UX_AbilitySystemComponent::CheckGameplayEffectHasMatchingAssetTags(UAbilitySystemComponent* ASC,
                                                                           const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	
	// Server-side filter guard: aborts RPC dispatch if no UI tags exist.
	if (TagContainer.HasTag(XGameplayTags::UI_Message))
	{
		Client_GameplayEffectAppliedToSelf(TagContainer);
	}
	
}

void UX_AbilitySystemComponent::Client_GameplayEffectAppliedToSelf_Implementation(const FGameplayTagContainer& AssetTags)
{
	OnIncomingGameplayEffectAssetTags.Broadcast(AssetTags);
}

void UX_AbilitySystemComponent::AbilityInputTagPressedAndHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTag(InputTag))
		{
			// Notifies the Ability System that this Gameplay Ability's input is currently in the Pressed state.
			AbilitySpecInputPressed(AbilitySpec);

			// Try and activate the Ability if it isn't already active.
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UX_AbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTag(InputTag))
		{
			// Notifies the Ability System that this Gameplay Ability's input has entered the Released state.
			AbilitySpecInputReleased(AbilitySpec);
			
		}
	}
}

