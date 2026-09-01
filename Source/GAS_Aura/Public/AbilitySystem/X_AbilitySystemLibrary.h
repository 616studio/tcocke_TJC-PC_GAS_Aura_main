// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "X_AbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
struct FModelsPayload;
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
	 * Retrieves the Controller specific to <c>View_AttributeMenu</c> by routing to the HUD through the local Player Controller.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Decoupling:</b> Used to avoid creating a hard dependency to the HUD by routing through PlayerController, which has access to the HUD by default.</description></item>
	 * <item><description><b>Global Access:</b> Can be called from within any Blueprint that has a valid World Context.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="WorldContextObject">[<c>UObject*</c>]: The context object (usually <c>self</c> in Blueprint) used to find the current game world.</param>
	 * <returns>[<c>UX_UI_Controller_AttributeMenu*</c>]: The initialized Controller specific to <c>View_AttributeMenu</c>.</returns>
	 */
	UFUNCTION(BlueprintPure, Category = "***CUSTOM|GAS|UI", meta = (DefaultToSelf = "WorldContextObject"))
	static UX_UI_Controller_AttributeMenu* GetView_AttributeMenuController(UObject* WorldContextObject);
	
	/**
	 * <summary>
	 * Retrieves the Controller specific to <c>View_HUD</c> by routing to the HUD through the local Player Controller.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Decoupling:</b> Used to avoid creating a hard dependency to the HUD by routing through PlayerController, which has access to the HUD by default.</description></item>
	 * <item><description><b>Global Access:</b> Can be called from within any Blueprint that has a valid World Context.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="WorldContextObject">[<c>UObject*</c>]: The context object (usually <c>self</c> in Blueprint) used to find the current game world.</param>
	 * <returns>[<c>UX_UI_Controller_HUD*</c>]: The initialized Controller specific to <c>View_HUD</c>.</returns>
	 */
	UFUNCTION(BlueprintPure, Category = "***CUSTOM|GAS|UI", meta = (DefaultToSelf = "WorldContextObject"))
	static UX_UI_Controller_HUD* GetView_HUDController(UObject* WorldContextObject);
	
	/**
	 * <summary>
	 * Returns whether a hit was blocked or not so the Player's UI can be updated to show the relevant message.
	 * </summary> 
	 */
	//UFUNCTION(BlueprintPure, Category = "***CUSTOM|GAS")
	//static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	/**
	 * <summary>
	 * Returns whether a hit was critical or not so the Player's UI can be updated to show the relevant message.
	 * </summary> 
	 */
	//UFUNCTION(BlueprintPure, Category = "***CUSTOM|GAS")
	//static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	/**
	 * <summary>
	 * Sets the Blocked Hit flag inside the provided Gameplay Effect Context.
	 * </summary>
	 * <remarks>
	 * <list type="bullet">
	 * <item><description><b>UPARAM:</b> Instructs Unreal's Blueprint system to treat the Context Handle as a pass-by-reference input pin rather than an output pin.</description></item>
	 * </list>
	 * </remarks>
	 */
	//UFUNCTION(BlueprintCallable, Category = "***CUSTOM|GAS")
	//static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	/**
	 * <summary>
	 * Sets the Critical Hit flag inside the provided Gameplay Effect Context.
	 * </summary>
	 * <remarks>
	 * <list type="bullet">
	 * <item><description><b>UPARAM:</b> Instructs Unreal's Blueprint system to treat the Context Handle as a pass-by-reference input pin rather than an output pin.</description></item>
	 * </list>
	 * </remarks>
	 */
	//UFUNCTION(BlueprintCallable, Category = "***CUSTOM|GAS")
	//static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
	
protected:
	
private:
	
	/**
	 * <summary>
	 * Helper function to consolidate the boilerplate logic of retrieving the HUD and the 4 Models:  PlayerController, PlayerState, ASC, and AttributeSet.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Prevents copying and pasting the exact same null-checking logic into every single Controller getter.</description></item>
	 * <item><description>Populates the <c>OutHUD</c> and <c>OutPayload</c> references if successful, allowing the caller to immediately use them.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="WorldContextObject">[<c>UObject*</c>]: The context object used to trace back to the local player.</param>
	 * <param name="OutHUD">[<c>AX_HUD*&</c>]: The found HUD, passed out by reference.</param>
	 * <param name="OutPayload">[<c>FModelsPayload&</c>]: The fully built Model payload, passed out by reference.</param>
	 * <returns>[<c>bool</c>]: True if all Models were successfully found and initialized, false otherwise.</returns>
	 */
	static bool TryGetModels(UObject* WorldContextObject, AX_HUD*& OutHUD, FModelsPayload& OutPayload);
	
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
	
	/**
	 * <summary>
	 * Retrieves the Resistance Tag associated with a specific Damage Type Tag.
	 * </summary>
	 */
	//UFUNCTION(BlueprintPure, Category = "***CUSTOM|GAS")
	//static FGameplayTag GetResistanceTagForDamageType(UObject* WorldContextObject, const FGameplayTag& DamageTypeTag);

	/**
	 * <summary>
	 * Returns any actors that implement the <c>X_CharacterInterface</c> within the specified sphere radius that aren't dead.
	 * </summary> 
	 */
	//UFUNCTION(BlueprintCallable, Category = "***CUSTOM|GAS")
	//static void GetLivingCharactersWithinSphereRadius(UObject* WorldContextObject, TArray<AActor*>& OverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);
	
	/**
	 * <summary>
	 * Returns a reference to <c>X_CharacterClassInfo</c> so the caller does not need to be hardcoded to the Game Mode class where it is instantiated.
	 * </summary>
	 * <param name="WorldContextObject">[<c>UObject*</c>]: The context object (usually <c>self</c> in Blueprint) used to find the current game world.</param>
	 * <returns>[<c>UX_CharacterClassInfo*</c>]: Access to the Character Class default values.</returns>
	 */
	//UFUNCTION(BlueprintPure, Category = "***CUSTOM|Character Class Defaults", meta = (DefaultToSelf = "WorldContextObject"))
	//static UX_CharacterClassInfo* GetCharacterClassInfo(UObject* WorldContextObject);
	
protected:
	
private:
	
#pragma endregion Utility	
	
};
