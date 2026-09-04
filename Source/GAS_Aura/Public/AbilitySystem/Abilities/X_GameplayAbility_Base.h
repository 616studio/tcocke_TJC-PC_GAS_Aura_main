// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "X_GameplayAbility_Base.generated.h"

/**
 * <summary>
 * The custom base class for all Gameplay Abilities.
 * </summary>
 */
UCLASS()
class GAS_AURA_API UX_GameplayAbility_Base : public UGameplayAbility
{
	GENERATED_BODY()

public:

	/**
	 * <summary>
	 * Gameplay Tag of category <c>Custom.Input</c> used to bind any derived ability to a Player input (ex: <c>Custom.Input.LMB</c> or <c>Custom.Input.1</c>).
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Granting Phase:</b> Injected into the <c>FGameplayAbilitySpec</c> by the ASC upon Gameplay Ability being granted.</description></item>
	 * <item><description><b>Execution Phase:</b> Routes input via Gameplay Tag matching instead of legacy <c>InputID</c>.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Input")
	FGameplayTag GameplayAbilityInputActionTag;
	
};
