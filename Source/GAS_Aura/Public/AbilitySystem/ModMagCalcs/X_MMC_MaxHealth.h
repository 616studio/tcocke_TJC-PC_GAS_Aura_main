// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "X_MMC_MaxHealth.generated.h"

/**
 * <summary>
 * Custom calculation class used by Gameplay Effects to dynamically determine the MaxHealth Attribute based on Vigor and Character Level.
 * </summary>
 */
UCLASS()
class GAS_AURA_API UX_MMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	
	UX_MMC_MaxHealth();
	
	/**
	 * <summary>
	 * The core execution function where the mathematical calculation actually happens.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Automatically called by the Gameplay Ability System whenever a Gameplay Effect utilizing this MMC is applied to a target.</description></item>
	 * <item><description>Executes the formula:  MaxHealth = BaseValue + (VigorMultiplier * Vigor) + (LevelMultiplier * CharacterLevel)</description></item>
	 * </list>
	 * </remarks>
	 * <param name="Spec">[<c>const FGameplayEffectSpec&</c>]: The specific data payload of the Gameplay Effect triggering this calculation.</param>
	 * <returns>[<c>float</c>]: The final calculated magnitude to be applied to the Attribute.</returns>
	 */
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
protected:
	
private:
	
	/**
	 * <summary>
	 * Defines exactly which attribute this MMC needs to capture, who to capture it from, and when.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Instructs the MMC to capture the Target's Vigor Attribute.</description></item>
	 * </list>
	 * </remarks>
	 */
	FGameplayEffectAttributeCaptureDefinition TargetVigorDefinition;
	
	/**
	 * <summary>
	 * The minimum starting MaxHealth value before Vigor and Character Level are factored in.
	 * </summary>
	 */
	UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Calculations")
	float BaseValue = 80.0f;

	/**
	 * <summary>
	 * Represents the multiplier (X) for how MaxHealth scales with Vigor.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Multiplier (X) > 1.0:</b> X point(s) of MaxHealth granted for every 1 point of Vigor.</description></item>
	 * <item><description><b>Multiplier (X) < 1.0:</b> 1 point of MaxHealth granted for every (1/X) points of Vigor.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Calculations")
	float VigorMultiplier = 2.5f;

	/**
	 * <summary>
	 * Represents the multiplier (X) for how MaxHealth scales with the Character's Level.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Multiplier (X) > 1.0:</b> X point(s) of MaxHealth granted for every 1 Character Level.</description></item>
	 * <item><description><b>Multiplier (X) < 1.0:>/b> 1 point of MaxHealth granted for every (1/X) Character Levels.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Calculations")
	float LevelMultiplier = 10.0f;

};
