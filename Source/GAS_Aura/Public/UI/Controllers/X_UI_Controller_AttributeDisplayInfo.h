// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "X_UI_Controller_Base.h"
#include "GAS_Aura/UtilityClasses/X_CustomDelegates.h"
#include "X_UI_Controller_AttributeDisplayInfo.generated.h"

class UX_AttributeDisplayInfo;
/**
 * <summary>
 * Controller designed specifically for detecting Gameplay Attribute changes and broadcasting a fully constructed <c>FAttributeDisplayInfo</c> payload.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Subclassed in Blueprint as <c>BP_UI_Controller_AttributeDisplayInfo</c> to allow designers to set class variables in the Editor.</description></item>
 * </list>
 * </remarks>
 */
UCLASS()
class GAS_AURA_API UX_UI_Controller_AttributeDisplayInfo : public UX_UI_Controller_Base
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
	 * <item><description>Loops through the <c>AttributeDisplayInfoDataAsset</c> and routes each entry to helper function <c>BroadcastAttributeDisplayInfo</c>.</description></item>
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
	 * <item><description>Loops through the <c>AttributeDisplayInfoDataAsset</c> to bind each Attribute to the Model ASC's GAS default delegate <c>GetGameplayAttributeValueChangeDelegate</c> using <c>AddWeakLambda</c>.</description></item>
	 * <item><description>Replaces hardcoded delegate bindings with a scalable, data-driven loop, allowing new Attributes to be added by simply updating the Data Asset.</description></item>
	 * <item><description>Utilizes the <c>bCallbacksBound</c> flag to prevent duplicate delegate bindings if initialized multiple times.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void BindCallbacksToModelDelegates() override;
	
	/**
	 * <summary>
	 * Delegate responsible for broadcasting an <c>FAttributeDisplayInfo</c> payload.
	 * </summary>
	 */
	UPROPERTY(BlueprintAssignable, Category = "***CUSTOM|Attributes")
	FX_AttributeDisplayInfoSignature AttributeDisplayInfoDelegate;
	
protected:
	
	/**
	 * <summary>
	 * Holds a reference to the Data Asset <c>DA_AttributeDisplayInfo</c> that acts as the master database of UI display data for all project Attributes.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>The Data Asset class (<c>X_AttributeDisplayInfo</c>) is a <c>TArray</c> of <c>FAttributeDisplayInfo</c> structs.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UX_AttributeDisplayInfo> AttributeDisplayInfoDataAsset;
	
private:

	/**
	 * <summary>
	 * Helper function to consolidate the Attribute value extraction and broadcasting process involving the <c>FAttributeDisplayInfo</c> payload.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b> 
	 * <list type="bullet">
	 * <item><description><c>Info</c> is passed in by value and not reference to avoid having to create a local, editable copy of the payload in order to update its member variable <c>AttributeValue</c>.</description></item>
	 * <item><description>Extracts the numeric value of the specified Attribute using the native GAS <c>GetNumericValue</c> function.</description></item>
	 * <item><description>Updates payload with the extracted numeric value, and then broadcasts the updated payload using <c>AttributeDisplayInfoDelegate</c>.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="Info">[<c>FAttributeDisplayInfo</c>]: The struct payload containing the Gameplay Attribute we need to evaluate.</param>
	 */
	void BroadcastAttributeDisplayInfo(FAttributeDisplayInfo Info) const;
};
