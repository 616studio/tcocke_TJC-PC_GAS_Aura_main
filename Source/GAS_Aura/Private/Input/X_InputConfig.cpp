// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#include "Input/X_InputConfig.h"
#include "GAS_Aura/UtilityClasses/X_CustomDataTypes.h"

const UInputAction* UX_InputConfig::FindInputActionFromGameplayTag(const FGameplayTag& InputTag,
                                                                   bool bLogNotFound) const
{
	for (const FX_InputActionGameplayTag& AbilityInputAction : AbilityInputActions)
	{
		if (!AbilityInputAction.InputAction) continue;
		
		if (AbilityInputAction.InputTag.MatchesTagExact(InputTag))
		{
			return AbilityInputAction.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find InputAction for InputTag [%s] on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
