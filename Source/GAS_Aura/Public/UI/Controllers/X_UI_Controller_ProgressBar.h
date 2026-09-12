// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GAS_Aura/UtilityClasses/X_CustomDelegates.h"
#include "UI/Controllers/X_UI_Controller_Base.h"
#include "X_UI_Controller_ProgressBar.generated.h"

/**
 * <summary>
 * Retrieves the Current and Max value for the specified Attribute.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Designed to work with any Progress Bar View.</description></item>
 * <item><description>Subclassed in Blueprint as <c>BP_UI_Controller_ProgressBar_AttributeName</c> to allow designers to set class variables in the Editor.</description></item>
 * </list>
 * </remarks>
 */
UCLASS(BlueprintType)
class GAS_AURA_API UX_UI_Controller_ProgressBar : public UX_UI_Controller_Base
{
	GENERATED_BODY()
	
public:
	
	/**
	 * <summary>
	 * Overriden derived class implementation used to perform a manual broadcast using any of the Model values assigned to this Controller.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Calls native GAS function <c>GetNumericAttribute</c> against the ASC Model for the Current and Max values of the specified Attribute.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void BroadcastCurrentModelValues() override;
	
	/**
	 * <summary>
	 * Overridden derived class implementation used to specify the callbacks this Controller needs to bind to any of the Model delegates.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Calls native GAS function <c>GetGameplayAttributeValueChangeDelegate</c> against the ASC Model for the Current and Max values of the specified Attribute.</description></item>
	 * <item><description>Utilizes the <c>bCallbacksBound</c> flag to prevent duplicate delegate bindings if initialized multiple times.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void BindCallbacksToModelDelegates() override;
	
protected:
	
	/**
	 * <summary>
	 * The Current value Gameplay Attribute to be captured.
	 * </summary>
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "***CUSTOM|Attributes")
	FGameplayAttribute CurrentValueAttribute;
	
	/**
	 * <summary>
	 * The Max value Gameplay Attribute to be captured.
	 * </summary>
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "***CUSTOM|Attributes")
	FGameplayAttribute MaxValueAttribute;

	/**
	 * <summary>
	 * Delegate used to broadcast a consolidated payload for a Gameplay Attribute that includes its Current and Max value.
	 * </summary>
	 */
	UPROPERTY(BlueprintAssignable, Category = "***CUSTOM|Attributes")
	FX_AttributeDisplayProgressBarSignature OnAttributeValueChanged;

	/**
	 * <summary>
	 * Consolidated Gameplay Attribute payload that includes its Current and Max value.
	 * </summary>
	 */
	UPROPERTY(BlueprintReadOnly, Category = "***CUSTOM|Attributes")
	FX_AttributeDisplayProgressBar AttributeInfo;
};
