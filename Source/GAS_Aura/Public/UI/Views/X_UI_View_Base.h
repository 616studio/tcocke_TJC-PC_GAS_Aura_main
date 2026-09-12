// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/X_UI_ViewInterface.h"
#include "X_UI_View_Base.generated.h"

class UX_UI_Controller_Base;
/**
 * <summary>
 * The abstract base View in the MVC (Model-View-Controller) UI Architecture.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description><b>Visual Only:</b> Performs no calculations or logic and stores no game states.</description></item>
 * <item><description><b>Implementation:</b> Designed to be subclassed exclusively as Widget Blueprints (<c>WBPs</c>) in the Editor.</description></item>
 * </list>
 * </remarks>
 * 
 */
UCLASS(Abstract)
class GAS_AURA_API UX_UI_View_Base : public UUserWidget, public IX_UI_ViewInterface
{
	GENERATED_BODY()
	
public:
	
	/**
	 * <summary>
	 * <c>IX_UI_ViewInterface</c> implementation.  Used by external callers to communicate to this View it is safe to start its initialization process.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Can also be used by external callers to force a View to refresh its data without requiring a hard cast to the View's class type.</description></item>
	 * <item><description>Triggers internal Blueprint Event <c>ReadyToInitialize</c>.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void PerformInitialization_Implementation() override;
	
	/**
	 * <summary>
	 * Returns the specific child widget that should receive Slate focus when this View becomes active.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Used to prevent hard coupling between parent Views and child Views when switching between them based on UI navigation buttons.</description></item>
	 * </list>
	 * </remarks>
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "***CUSTOM|UI")
	UWidget* GetDesiredFocusWidget();
	virtual UWidget* GetDesiredFocusWidget_Implementation() { return this; }

	/**
	 * <summary>
	 * Tracks whether this View has successfully retrieved its UI Controller and has bound to its Model delegates.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Used to handle asynchronous network replication issues between the HUD, PlayerState, and Models to ensure this View is properly initialized.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(BlueprintReadWrite, Category = "***CUSTOM|UI")
	bool bIsInitialized = false;
	
protected:
	
	/**
	 * <summary>
	 * Native UE5 Slate teardown hook. Executed when the widget is removed from the viewport or parent panel.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Sets <c>bIsInitialized</c> to false by default for all Views to ensure it is set even if a Blueprint graph deletes or forgets the Parent: Destruct node.</description></item>
	 * <item><description><b>IMPORTANT:</b> Derived Views <b>MUST</b> call the Parent Destruct node if they implement their own logic (ex:  unbinding from delegates).</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void NativeDestruct() override;

	/**
	 * <summary>
	 * Blueprint event triggered immediately after a View has been instructed it is safe to initialize itself via <c>PerformInitialization</c>.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Used to notify a View (or its children connected via its Designer) in its Event Graph it is safe to retrieve their Controller to bind their callbacks to its delegate(s).</description></item>
	 * <item><description><b>Timing Warning:</b> Unreal's native <c>Construct</c> event will always fire before the Controller is assigned. Attempting to bind delegates during <c>Construct</c> will cause null-pointer crashes.</description></item>
	 * </list>
	 * </remarks>
	 */	
	UFUNCTION(BlueprintImplementableEvent, Category = "***CUSTOM|UI")
	void ReadyToInitialize();
	
private:
	
};
