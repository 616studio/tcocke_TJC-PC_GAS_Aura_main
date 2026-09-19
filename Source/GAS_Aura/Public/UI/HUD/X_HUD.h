// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GAS_Aura/UtilityClasses/X_CustomDataTypes.h"
#include "UI/Controllers/X_UI_Controller_Base.h"
#include "X_HUD.generated.h"

// Forward declarations.
struct FX_ModelsPayload;
class UX_UI_Controller_Base;
class UAbilitySystemComponent;
class UAttributeSet;
class UX_UI_View_Base;
class UX_UI_Controller_HUD;
class UX_UI_Controller_AttributeMenu;
class UX_UI_Controller_ScreenMessage_ItemPickup;

/**
 * <summary>
 * Assembles the View, Controller, and Model layers in the MVC (Model-View-Controller) UI architecture for the Player.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Subclassed in Blueprint as <c>BP_HUD</c> to allow designers to set class variables in the Editor.</description></item>
 * </list>
 * </remarks>
 */
UCLASS()
class GAS_AURA_API AX_HUD : public AHUD
{
	GENERATED_BODY()

public:
	
	/**
     * <summary>
     * Called by <c>X_Character_Player</c> to start the HUD initialization process.
     * </summary>
     * <remarks>
     * <b>ARCHITECTURE NOTES:</b>
     * <list type="bullet">
	 * <item><description><b>Network Safety:</b> Defers instantiation until required Models have replicated to the Client.</description></item>
	 * <item><description><b>State Synchronization:</b> Reassigns Models on every call to prevent Views from tracking stale data after a Pawn repossession. Delegate rebinding requests are safely caught and ignored by the Controller's internal state guards.</description></item>
	 * <item><description><b>Initialization Broadcast:</b> Broadcasts Model data to Views from their assigned Controllers every call to prevent empty UI states.</description></item>
     * </list>
     * </remarks>
     * <param name="PC">[<c>APlayerController*</c>]: (<b>Model</b>) The owning Player Controller.</param>
     * <param name="PS">[<c>APlayerState*</c>]: (<b>Model</b>) The owning Player State.</param>
     * <param name="ASC">[<c>UAbilitySystemComponent*</c>]: (<b>Model</b>) The generic base pointer to the Ability System Component.</param>
     * <param name="AS">[<c>UAttributeSet*</c>]: (<b>Model</b>) The generic base pointer to the Attribute Set.</param>
     */
	void InitHUD(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
	/**
	 * <summary>
	 * Public templated accessor that lazily fetches or instantiates a specialized UI Controller.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Lazy Instantiation:</b> Spawns the requested Controller on demand if it does not yet exist in <c>ControllerRegistry</c>.</description></item>
	 * <item><description><b>Type Safety:</b> Automatically casts the internal base controller pointer to requested type <c>T</c>.</description></item>
	 * <item><description>Used exclusively by <c>X_AbilitySystemLibrary::GetUIController</c> to ensure loose coupling from all external callers.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="T">The specialized target class type derived from <c>UX_UI_Controller_Base</c>.</param>
	 * <param name="ControllerClass">[<c>TSubclassOf<<UX_UI_Controller_Base>></c>]: The Blueprint or C++ class of the UI Controller to fetch or create.</param>
	 * <returns>[<c>T*</c>]: Strongly-typed pointer to the requested UI Controller instance.</returns>
	 */
	template <typename T = UX_UI_Controller_Base>
	T* GetOrCreateController(TSubclassOf<UX_UI_Controller_Base> ControllerClass)
	{
		return Cast<T>(GetOrCreateController_Internal(ControllerClass));
	}

protected:
	
	/**
	 * <summary>
	 * Internal worker function that performs registry lookup, instantiation, Model payload injection, and delegate initialization for UI Controllers.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Garbage Collection Anchor:</b> Newly created instances are immediately cached in <c>ControllerRegistry</c>.</description></item>
	 * <item><description><b>Payload Injection:</b> Automatically passes <c>CurrentModelsPayload</c> to new instances during lazy instantiation.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="ControllerClass">[<c>TSubclassOf<<UX_UI_Controller_Base>></c>]: The class type of the UI Controller to instantiate or fetch.</param>
	 * <returns>[<c>UX_UI_Controller_Base*</c>]: Base pointer to the created or cached UI Controller instance.</returns>
	 */
	UX_UI_Controller_Base* GetOrCreateController_Internal(TSubclassOf<UX_UI_Controller_Base> ControllerClass);
	
	/**
	 * <summary>
	 * Central map tracking all instantiated UI Controllers keyed by their class type.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Memory Management:</b> Acts as the primary <c>UPROPERTY()</c> root container holding hard references to prevent runtime Garbage Collection.</description></item>
	 * <item><description><b>Singleton Pattern:</b> Ensures only one instance of a specific UI Controller class exists per local Player HUD.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY()
	TMap<TSubclassOf<UX_UI_Controller_Base>, TObjectPtr<UX_UI_Controller_Base>> ControllerRegistry;
	
	/**
	 * <summary>
	 * Cached model context payload containing the active Player Controller, Player State, ASC, and Attribute Set.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Lazy Context Injection:</b> Passed directly into new Controllers during on-demand calls to <c>GetOrCreateController</c>.</description></item>
	 * <item><description><b>Respawn Updates:</b> Refreshed via <c>InitHUD</c> upon Pawn possession to prevent Controllers from holding stale Model references.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY()
	FX_ModelsPayload CurrentModelsPayload;

	/**
	 * <summary>
	 * The parent View that acts solely as a visual layer for positioning child Views.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>There is exactly one instance of this View per local Player.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY()
	TObjectPtr<UX_UI_View_Base> View_HUD;

private:
	
	/**
	 * <summary>
	 * Widget Blueprint class used to instantiate <c>View_HUD</c>.
	 * </summary>
	 */
	UPROPERTY(EditAnywhere, Category = "***CUSTOM|Classes|Views")
	TSubclassOf<UX_UI_View_Base> View_HUDClass;

};
