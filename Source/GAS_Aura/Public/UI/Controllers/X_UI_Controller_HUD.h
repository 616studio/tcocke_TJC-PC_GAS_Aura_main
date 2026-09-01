// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "X_UI_Controller_Base.h"
#include "UI/X_UI_Delegates.h"
#include "X_UI_Controller_HUD.generated.h"

class UX_AttributeSet;
class UX_AbilitySystemComponent;
/**
 * <summary>
 * The Controller designed specifically for use with the HUD's <c>View_HUD</c>.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE RULES:</b>
 * <list type="bullet">
 * <item><description><b>BlueprintType:</b> Allows casting to this specific Controller type inside Blueprints.</description></item>
 * </list>
 * </remarks>
 */
UCLASS(BlueprintType)
class GAS_AURA_API UX_UI_Controller_HUD : public UX_UI_Controller_Base
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
	 * <item><description>Extracts the Vital Attribute values (and their associated Secondary Attribute values) from the downcast Model <c>AS</c> and broadcasts them via their respective delegate instances.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void BroadcastCurrentModelValues() override;

	/**
	 * <summary>
	 * Overriden derived class implementation used to specify the callbacks it wants to bind to any of the Model delegates.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Validates the downcast <c>UX_AttributeSet</c> and <c>UX_AbilitySystemComponent</c> pointers.</description></item>
	 * <item><description>Routes the validated pointers to the <c>BindToAttributeDelegate()</c> and <c>BindToGameplayEffectAssetTagsDelegate()</c> helper functions.</description></item>
	 * <item><description>Utilizes the <c>bCallbacksBound</c> flag to prevent duplicate delegate bindings if initialized multiple times.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void BindCallbacksToModelDelegates() override;
	
	protected:
	
private:

#pragma region Attribute Value Delegate Functionality

public:
	
	// Attribute delegate instances.
	
	UPROPERTY(BlueprintAssignable, Category = "***CUSTOM|UI")
	FX_UIAttributeDisplayCurrentMaxPayloadSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "***CUSTOM|UI")
	FX_UIAttributeDisplayCurrentMaxPayloadSignature OnManaChanged;
	
protected:
	
private:
	
	/**
	 * <summary>
	 * Helper function to consolidate the Attribute Value delegate binding logic.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Binds this Controller to the downcast Model ASC's <c>GetGameplayAttributeValueChangeDelegate</c> for each Attribute using <c>AddWeakLambda</c>.</description></item>
	 * <item><description><c>FOnGameplayAttributeValueChange</c> is the internal GAS notification that fires whenever the Base or Current Value of an Attribute is modified.</description></item>
	 * <item><description>When a change is detected, the <c>NewValue</c> is extracted from the <c>FOnAttributeChangeData</c> payload.</description></item>
	 * <item><description>Each Attribute is assigned an instance of our custom delegate used to broadcast the <c>NewValue</c> to its listeners.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="XAS">[<c>UX_AttributeSet*</c>]: The downcast Model AS pointer that provides the Attribute definitions.</param>
	 * <param name="XASC">[<c>UX_AbilitySystemComponent*</c>]: The downcast Model ASC pointer used to establish the delegate bindings.</param>
	 */
	void BindToAttributeDelegates(UX_AttributeSet* XAS, UX_AbilitySystemComponent* XASC);
	
#pragma endregion Attribute Value Delegate Functionality

#pragma region Screen Message Row Delegate Functionality

public:
	
	/**
	 * <summary>
	 * Delegate responsible for broadcasting an <c>FScreenMessageItemPickupRowStructure</c> from the <c>DataTableScreenMessageItemPickup</c>.
	 * </summary>
	 */
	UPROPERTY(BlueprintAssignable, Category = "***CUSTOM|UI")
	FX_ScreenMessageItemPickupSignature OnScreenMessageItemPickup;

protected:

	/**
	 * <summary>
	 * The Data Table asset containing <c>FScreenMessageItemPickupRowStructure</c> entries.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Exists as asset <c>DT_ScreenMessage_ItemPickup</c>.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|UI")
	TObjectPtr<UDataTable> DataTableScreenMessageItemPickup;
	
private:
	
	/**
	 * <summary>
	 * Helper function to consolidate the <c>OnScreenMessageItemPickup</c> delegate binding logic.	 
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Binds this Controller to the downcast Model ASC's <c>OnIncomingGameplayEffectAssetTags</c> delegate using <c>AddWeakLambda</c>.</description></item>
	 * <item><description><c>OnIncomingGameplayEffectAssetTags</c> broadcasts the Asset Tags of any Gameplay Effect that gets applied to the associated ASC.</description></item>
	 * <item><description>When a Gameplay Tag is received from <c>OnIncomingGameplayEffectAssetTags</c>, queries the <c>DataTableScreenMessageItemPickup</c> for a row that matches the Gameplay Tag.</description></item>
	 * <item><description>If a matching row is found, <c>OnScreenMessageItemPickup</c> is used to broadcast the <c>FScreenMessageItemPickupRowStructure</c> payload to its listeners.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="XASC">[<c>UX_AbilitySystemComponent*</c>]: The downcast Model ASC pointer used to establish the delegate binding.</param>
	 */
	void BindToGameplayEffectAssetTagsDelegate(UX_AbilitySystemComponent* XASC);
	
#pragma endregion Screen Message Row Delegate Functionality

};
