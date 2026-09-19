// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "X_AbilitySystemLibrary.generated.h"

class UWidget;
class UX_UI_Controller_Base;
class UX_CharacterClassInfo;
struct FGameplayEffectContextHandle;
struct FX_ModelsPayload;
class AX_HUD;
class UX_UI_Controller_HUD;
class UX_UI_Controller_AttributeMenu;

/**
 * <summary>
 * Provides global utility functions for the Gameplay Ability System and UI infrastructure.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description><b>World Context:</b> Requires a <c>WorldContextObject</c> to route calls to the active Game World.</description></item>
 * </list>
 * </remarks>
 */
UCLASS()
class GAS_AURA_API UX_AbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
#pragma region UI
	
public:
	
	/**
	* <summary>
	 * Global accessor that routes through the local Player Controller and HUD to fetch or create a UI Controller.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Decoupled View Lookup:</b> Enables Views to retrieve their dedicated UI Controllers without storing hard references to HUD instances.</description></item>
	 * <item><description><b>World Tracing:</b> Resolves the local <c>APlayerController</c> using <c>GetPlayerControllerFromWorldContextObject</c> before querying <c>AX_HUD</c>.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="WorldContextObject">[<c>UObject*</c>]: The context object (Widget, Actor, or State) used to resolve the local Player Controller.</param>
	 * <param name="ControllerClassToAssign">[<c>TSubclassOf<UX_UI_Controller_Base></c>]: The class type of the requested UI Controller.</param>
	 * <returns>[<c>UX_UI_Controller_Base*</c>]: Pointer to the cached or freshly instantiated UI Controller, or <c>nullptr</c> if unreachable.</returns>
	 */
	UFUNCTION(BlueprintPure, Category = "***CUSTOM|GAS|UI", meta = (DefaultToSelf = "WorldContextObject", DeterminesOutputType = "ControllerClassToAssign"))
	static UX_UI_Controller_Base* GetUIController(UObject* WorldContextObject, TSubclassOf<UX_UI_Controller_Base> ControllerClassToAssign);
	
	/**
	 * <summary>
	 * Recursively traverses a UMG widget tree and invokes PerformInitialization on any View implementing IX_UI_ViewInterface.
	 * </summary>
	 * <param name="RootWidget">[<c>UWidget*</c>]: The top-level container or root panel to begin recursive traversal from.</param>
	 */
	UFUNCTION(BlueprintCallable, Category = "***CUSTOM|GAS|UI", meta = (DefaultToSelf = "RootWidget"))
	static void InitializeChildViews(UWidget* RootWidget);
	
protected:
	
private:
	
#pragma endregion UI
	
#pragma region Utility
	
public:
	
	/**
	 * <summary>
	 * Used to find the specific Player Controller associated with the provided context object.
	 * </summary>
	 * <param name="WorldContextObject">[<c>UObject*</c>]: The context object (Widget, Actor, or State) to trace back to a Player Controller.</param>
	 * <returns>[<c>APlayerController*</c>]: The found Player Controller, or <c>nullptr</c> if one could not be found.</returns>
	 */
	UFUNCTION(BlueprintPure, Category = "***CUSTOM|GAS", meta = (DefaultToSelf = "WorldContextObject"))
	static APlayerController* GetPlayerControllerFromWorldContextObject(UObject* WorldContextObject);
	
	/**
	 * <summary>
	 * Returns a specific row from the supplied Data Table that corresponds to the supplied Gameplay Tag.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Header Definition:</b> Since this is a C++ template, the definition <b>MUST</b> live directly in the header file.</description></item>
	 * <item><description><b>C++ Only:</b> Unreal Header Tool (UHT) does not allow templates to be <c>UFUNCTION</c>s. This function is not accessible via Blueprints.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="DataTable">[<c>UDataTable*</c>]: The specific Data Table asset to search within.</param>
	 * <param name="Tag">[<c>const FGameplayTag&</c>]: The exact Gameplay Tag acting as the Row Name.</param>
	 * <returns>[<b>T*</b>]: A pointer cast to the row structure, or <c>nullptr</c> if not found/invalid.</returns>
	 */
	template<typename T>
	static T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
	{
		// Safety check to prevent crashing if the designer forgot to assign the Data Table.
		if (!IsValid(DataTable)) return nullptr;
        
		return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
	}
	
protected:
	
private:
	
#pragma endregion Utility	
	
};
