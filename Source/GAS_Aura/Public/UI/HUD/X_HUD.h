// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "X_HUD.generated.h"

// Forward declarations.
struct FModelsPayload;
class UAbilitySystemComponent;
class UAttributeSet;
class UX_UI_View_Base;
class UX_UI_Controller_HUD;
class UX_UI_Controller_AttributeMenu;

/**
 * <summary>
 * Assembles the View, Controller, and Model layers in the MVC (Model-View-Controller) UI architecture for the Player.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Instantiates and owns <c>View_HUD</c> which contains all child Views specific to the local Player's viewport.</description></item>
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
	 * Instantiates and retrieves the fully initialized Controller (Models assigned and callbacks bound) for <c>View_HUD</c>.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Lazy Initialization:</b> Instantiates the Controller on first access.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="Models">[<c>const FModelsPayload&</c>]: payload struct containing pointers to the Models (<c>PC</c>, <c>PS</c>, <c>ASC</c>, and <c>AS</c>).</param>
	 */
	UX_UI_Controller_HUD* GetView_HUDController(const FModelsPayload& Models);

	/**
	 * <summary>
	 * Instantiates and retrieves the fully initialized Controller (Models assigned and callbacks bound) for <c>View_AttributeMenu</c>.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Lazy Initialization:</b> Instantiates the Controller on first access.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="Models">[<c>const FModelsPayload&</c>]: payload struct containing pointers to the Models (<c>PC</c>, <c>PS</c>, <c>ASC</c>, and <c>AS</c>).</param>
	 */
	UX_UI_Controller_AttributeMenu* GetView_AttributeMenuController(const FModelsPayload& Models);
	
	/**
     * <summary>
     * Initializes <c>View_HUD</c>.
     * </summary>
     * <remarks>
     * <b>ARCHITECTURE NOTES:</b>
     * <list type="bullet">
	 * <item><description><b>Network Safety:</b> Defers instantiation until required Models have replicated to the Client.</description></item>
	 * <item><description><b>State Synchronization:</b> Reassigns Models on every call to prevent <c>View_HUD</c> from tracking stale data after a Pawn repossession. Delegate rebinding requests are safely caught and ignored by the Controller's internal state guard.</description></item>
	 * <item><description><b>Initialization Broadcast:</b> Broadcasts Model data to <c>View_HUD</c> from its assigned Controller every call to prevent empty UI states.</description></item>
     * </list>
     * </remarks>
     * <param name="PC">[<c>APlayerController*</c>]: (<b>Model</b>) The owning Player Controller.</param>
     * <param name="PS">[<c>APlayerState*</c>]: (<b>Model</b>) The owning Player State.</param>
     * <param name="ASC">[<c>UAbilitySystemComponent*</c>]: (<b>Model</b>) The generic base pointer to the Ability System Component.</param>
     * <param name="AS">[<c>UAttributeSet*</c>]: (<b>Model</b>) The generic base pointer to the Attribute Set.</param>
     */
	void InitView_HUD(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:

	/**
	 * <summary>
	 * Holds the instantiated <c>View_HUD</c> that contains all child Views specific to the local Player's viewport.
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

	/**
	 * <summary>
	 * Holds the instantiated Controller for <c>View_HUD</c>.
	 * </summary>
	 */
	UPROPERTY()
	TObjectPtr<UX_UI_Controller_HUD> View_HUDController;

	/**
	 * <summary>
	 * Holds the instantiated Controller for <c>View_AttributeMenuView</c>.
	 * </summary>
	 */
	UPROPERTY()
	TObjectPtr<UX_UI_Controller_AttributeMenu> View_AttributeMenuController;

private:
	
	/**
	 * <summary>
	 * Widget Blueprint class used to instantiate <c>View_HUD</c>.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Class Default Object (CDO) Copies:</b> When NewObject instantiates a class from a TSubclassOf variable, it duplicates the Class Default Object (CDO) of that specific class. Subclassing in Blueprint allows the CDO to store serialized asset pointers assigned in the Editor.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditAnywhere, Category = "***CUSTOM|Classes|Views")
	TSubclassOf<UX_UI_View_Base> View_HUDClass;

	/**
	 * <summary>
	 * Blueprint class used to instantiate the Controller for <c>View_HUD</c>.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Class Default Object (CDO) Copies:</b> When NewObject instantiates a class from a TSubclassOf variable, it duplicates the Class Default Object (CDO) of that specific class. Subclassing in Blueprint allows the CDO to store serialized asset pointers assigned in the Editor.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditAnywhere, Category = "***CUSTOM|Classes|Controllers")
	TSubclassOf<UX_UI_Controller_HUD> View_HUDControllerClass;

	/**
	 * <summary>
	 * Blueprint class used to instantiate the Controller for <c>View_AttributeMenu</c>.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Class Default Object (CDO) Copies:</b> When NewObject instantiates a class from a TSubclassOf variable, it duplicates the Class Default Object (CDO) of that specific class. Subclassing in Blueprint allows the CDO to store serialized asset pointers assigned in the Editor.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditAnywhere, Category = "***CUSTOM|Classes|Controllers")
	TSubclassOf<UX_UI_Controller_AttributeMenu> View_AttributeMenuControllerClass;
	
};
