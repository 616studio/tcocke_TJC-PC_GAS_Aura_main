// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

// Forward declarations
struct FGameplayTag;

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "X_FloatingDamageTextWidget.generated.h"

/**
 * <summary>
 * Stateless UI implementation of floating damage text.
 * </summary>
 */
UCLASS()
class GAS_AURA_API UX_FloatingDamageTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	/**
	 * <summary>
	 * Initializes the widget with the damage data and its 3D origin point.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Implemented in Blueprint so designers can easily drive animations, colors, and the <c>ProjectWorldToScreen</c> math.</description></item>
	 * </list>
	 * </remarks>
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "***CUSTOM|Damage Text")
	void SetupFloatingDamageText(const float DamageAmount, const bool bIsMyDamage, const FVector Target3DLocation, const FGameplayTag MessageTag, const FGameplayTag AbilityTag);
};
