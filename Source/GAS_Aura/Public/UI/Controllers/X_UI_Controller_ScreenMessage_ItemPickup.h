// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "X_UI_Controller_Base.h"
#include "GAS_Aura/UtilityClasses/X_CustomDelegates.h"
#include "X_UI_Controller_ScreenMessage_ItemPickup.generated.h"

class UX_AbilitySystemComponent;
/**
 * <summary>
 * The Controller designed specifically for use with <c>WBP_View_ScreenMessage_ItemPickup</c> and managed by <c>WBP_View_HUD</c>.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Subclassed in Blueprint as <c>BP_UI_Controller_ScreenMessage_ItemPickup</c> to allow designers to set class variables in the Editor.</description></item>
 * </list>
 * </remarks>
 */
UCLASS(BlueprintType)
class GAS_AURA_API UX_UI_Controller_ScreenMessage_ItemPickup : public UX_UI_Controller_Base
{
	GENERATED_BODY()
	
public:
	
	/**
	 * <summary>
	 * Overriden derived class implementation used to specify the callbacks this Controller needs to bind to any of the Model delegates.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Calls helper function <c>BindToGameplayEffectAssetTagsDelegate</c>.</description></item>
	 * <item><description>Utilizes the <c>bCallbacksBound</c> flag to prevent duplicate delegate bindings if initialized multiple times.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void BindCallbacksToModelDelegates() override;
	
	/**
	 * <summary>
	 * Delegate responsible for broadcasting an <c>FScreenMessageItemPickupRowStructure</c> from the <c>DataTableScreenMessageItemPickup</c>.
	 * </summary>
	 */
	UPROPERTY(BlueprintAssignable, Category = "***CUSTOM|UI")
	FX_ScreenMessageItemPickupSignature OnScreenMessageItemPickupDelegate;
	
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
	
	/**
	 * <summary>
	 * Asynchronously preloads all <c>TSoftClassPtr</c> and <c>TSoftObjectPtr</c> references declared inside <c>DataTableScreenMessageItemPickup</c>.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Hitch Mitigation:</b> Eliminates Game Thread stalls (hitching) during active gameplay by streaming soft asset references into RAM before item pickup events fire.</description></item>
	 * <item><description><b>Streamable Manager Integration:</b> Utilizes <c>FStreamableManager</c> through the global <c>UAssetManager</c> singleton to execute non-blocking disk I/O background reads.</description></item>
	 * </list>
	 * </remarks>
	 */
	void PreloadScreenMessageDataTableAssets();
	
	/**
	 * <summary>
	 * Asynchronous completion callback executed when <c>UAssetManager</c> finishes streaming the soft asset paths requested by <c>PreloadScreenMessageDataTableAssets</c>.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Garbage Collection Anchoring:</b> Resolves streamed paths into hard pointers and caches them in <c>PreloadedScreenMessageAssets</c> (<c>UPROPERTY</c>) to prevent GC eviction during gameplay.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="PreloadedPaths">[<c>TArray of FSoftObjectPath</c>]: The collection of soft object paths that were asynchronously loaded into memory.</param>
	 */
	void OnScreenMessageAssetsPreloaded(TArray<FSoftObjectPath> PreloadedPaths);
	
	/** 
	 * <summary>
	 * Holds hard UPROPERTY references to preloaded UI assets to prevent GC collection during gameplay.
	 * </summary>
	 */
	UPROPERTY()
	TArray<TObjectPtr<UObject>> PreloadedScreenMessageAssets;
	
private:
	
	/**
	 * <summary>
	 * Helper function to consolidate the <c>OnScreenMessageItemPickup</c> delegate binding logic.	 
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Binds this Controller to the downcast Model ASC's <c>OnIncomingGameplayEffectAssetTags</c> delegate using <c>AddWeakLambda</c>.</description></item>
	 * <item><description>When a Gameplay Tag is received from <c>OnIncomingGameplayEffectAssetTags</c>, queries the <c>DataTableScreenMessageItemPickup</c> for a row that matches the Gameplay Tag.</description></item>
	 * <item><description>If a matching row is found, <c>OnScreenMessageItemPickup</c> is used to broadcast the <c>FScreenMessageItemPickupRowStructure</c> payload to its listeners.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="XASC">[<c>UX_AbilitySystemComponent*</c>]: The downcast Model ASC pointer used to establish the delegate binding.</param>
	 */
	void BindToGameplayEffectUIMessageAssetTagsDelegate(UX_AbilitySystemComponent* XASC);
};
