// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "X_UI_View_Base.generated.h"

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
class GAS_AURA_API UX_UI_View_Base : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	/**
	 * <summary>
	 * Cached reference to the current Controller assigned to this View.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Class Agnostic:</b> Declared as a <c>UObject</c> to allow Views to accept any specialized Controller (HUD, Inventory, Dialog).</description></item>
	 * <item><description><b>Do NOT Overwrite:</b> Must be assigned dynamically at runtime exclusively via <c>AssignControllerToView</c> to ensure the <c>ControllerHasBeenAssigned</c> Blueprint Event is triggered.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "***CUSTOM|UI")
	TObjectPtr<UObject> CurrentlyAssignedController;
	
	/**
	 * <summary>
	 * Assigns a Controller to this View.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Caches the Controller internally and immediately triggers the <c>ControllerHasBeenAssigned</c> Blueprint Event.</description></item>
	 * </list>
	 * </remarks>
	 */
	UFUNCTION(BlueprintCallable, Category = "***CUSTOM|UI")
	void AssignControllerToView(UObject* ControllerToAssign);
	
protected:

	/**
	 * <summary>
	 * Blueprint event triggered immediately after a Controller is successfully assigned to this View.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Used to notify a View it is safe to bind its internal callbacks to the assigned Controller's delegates in its own Event Graph.</description></item>
	 * <item><description><b>Timing Warning:</b> Unreal's native <c>Construct</c> event will always fire before the Controller is assigned. Attempting to bind delegates during <c>Construct</c> will cause null-pointer crashes.</description></item>
	 * </list>
	 * </remarks>
	 */	
	UFUNCTION(BlueprintImplementableEvent, Category = "***CUSTOM|UI")
	void ControllerHasBeenAssigned();
	
private:
	
};
