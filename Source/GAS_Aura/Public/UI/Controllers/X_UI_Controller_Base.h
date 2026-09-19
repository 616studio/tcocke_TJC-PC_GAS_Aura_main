// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "X_UI_Controller_Base.generated.h"


// Forward declarations.
struct FX_ModelsPayload;
class UAbilitySystemComponent;
class UAttributeSet;
class APlayerController;
class APlayerState;

/**
 * <summary>
 * The abstract base Controller in the MVC (Model-View-Controller) UI architecture.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>By deriving from <c>UObject</c>, any Controller can function purely as a lightweight data distributor, not restricted to a particular Engine type.</description></item>
 * <item>
 * <description>Establishes the strict three-step initialization pipeline for derived Controllers: 
 * <list type="bullet">
 * <item><description><b>Step 1 - Assign Models:</b> Assign the core Models to itself.</description></item>
 * <item><description><b>Step 2 - Bind Callbacks:</b> Bind its internal callbacks to Model delegates.</description></item>
 * <item><description><b>Step 3 - Broadcast Values:</b> Broadcast its current Model data via its own delegates to all synchronized listeners.</description></item>
 * </list>
 * </description>
 * </item>
 * </list>
 * </remarks>
 */
UCLASS(Abstract, Blueprintable)
class GAS_AURA_API UX_UI_Controller_Base : public UObject
{
	GENERATED_BODY()
	
public:

	/**
	 * <summary>
	 * Extracts the individual Model pointers from <c>FX_ModelsPayload</c> and caches them internally.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Performs <b>[Step 1 - Assign Models]</b> of the initialization pipeline.</description></item>
	 * <item><description>Must be called immediately after instantiation (in C++ or Blueprints) to provide the Models required for <b>[Step 2 - Bind Callbacks]</b>.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="Models">[<c>const FX_ModelsPayload&</c>]: Struct payload containing pointers to the Models (<c>PC</c>, <c>PS</c>, <c>ASC</c>, and <c>AS</c>).</param>
	 */
	UFUNCTION(BlueprintCallable)
	void AssignModelsToController(const FX_ModelsPayload& Models);

	/**
	 * <summary>
	 * Allows a derived Controller class to bind its own callbacks to any of the Model delegates.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Performs <b>[Step 2 - Bind Callbacks]</b> of the initialization pipeline for any Controller derived from this class.</description></item>
	 * <item><description>Downcasts Model pointers to access and subscribe to subclass-specific delegates.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void BindCallbacksToModelDelegates();
	
	/**
	 * <summary>
	 * Allows derived classes to perform a manual broadcast using any of the Model's assigned to this Controller.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Performs <b>[Step 3 - Broadcast Values]</b> of the initialization pipeline for any Controller derived from this class.</description></item>
	 * <item><description><b>Initialization:</b> Pre-populates listeners with current Model data prior to receiving their first dynamic broadcast.</description></item>
	 * <item><description><b>Force Sync:</b> Can be called during runtime to force listeners to synchronize their data with the Models.</description></item>
	 * </list>
	 * </remarks>
	 */
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastCurrentModelValues();

protected:

	/**
	 * <summary>
	 * Model cached as read-only during <b>[Step 1 - Assign Models]</b>.
	 * </summary>
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "***CUSTOM|UI")
	TObjectPtr<APlayerController> PlayerController;

	/**
	 * <summary>
	 * Model cached as read-only during <b>[Step 1 - Assign Models]</b>.
	 * </summary>
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "***CUSTOM|UI")
	TObjectPtr<APlayerState> PlayerState;

	/**
	 * <summary>
	 * Model cached as read-only during <b>[Step 1 - Assign Models]</b>.
	 * </summary>
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "***CUSTOM|UI")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/**
	 * <summary>
	 * Model cached as read-only during <b>[Step 1 - Assign Models]</b>.
	 * </summary>
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "***CUSTOM|UI")
	TObjectPtr<UAttributeSet> AttributeSet;

	/**
	 * <summary>
	 * Internal execution guard that prevents <b>[Step 2 - Bind Callbacks]</b> of the Controller initialization pipeline from executing more than once.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Guards against memory leaks caused by redundant delegate bindings when initialization is triggered multiple times (ex: network syncs or Player respawns).</description></item>
     * <item><description><b>Action Required:</b> Derived classes <b>MUST</b> check this flag at the top of their override and set it to true after binding!</description></item>
	 * </list>
	 * </remarks>
	 */
	bool bCallbacksBound = false;
};
