// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "X_InputConfig.generated.h"



// Forward declarations
struct FGameplayTag;
class UInputAction;
struct FX_InputActionGameplayTag;

/**
 * <summary>
 * Acts as the master database for all Input Actions mapped to Gameplay Tags.
 * </summary>
 */
UCLASS()
class GAS_AURA_API UX_InputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	/**
	 * <summary>
	 * Retrieves the Input Action associated with a Gameplay Tag.
	 * </summary>
	 * <param name="InputTag">[<c>const FGameplayTag&</c>]: The Gameplay Tag to search for.</param>
	 * <param name="bLogNotFound">[<c>bool</c>]: If set to <c>TRUE</c>, prints an error to the log if the Input Action could not be retrieved.</param>
	 * <returns>[<c>const UInputAction*</c>]: The found Input Action, or <c>nullptr</c> if not found.</returns>
	 */
	const UInputAction* FindInputActionFromGameplayTag(const FGameplayTag& InputTag, bool bLogNotFound) const;
	
	/**
	 * <summary>
	 * The array of Input Action to Gameplay Tag mappings defined within this Data Asset.
	 * </summary>
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="***CUSTOM|Input")
	TArray<FX_InputActionGameplayTag> AbilityInputActions;
};
