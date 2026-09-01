// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AttributeSet.h"
#include "Engine/DataTable.h"
#include "X_UI_DataTypes.generated.h"

// Forward declarations
class UTexture2D;

/**
 * <summary>
 * Defines the row structure used by the Data Table asset <c>DT_ScreenMessage_ItemPickup</c>.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Each row contains the text to be displayed (<c>Message</c>), the image to be displayed (<c>Image</c>), and the View class (<c>WBP_View_ScreenMessage_ItemPickup</c>) to be used to display the data.</description></item>
 * </list>
 * <b>IMPORTANT:</b>
 * <list type="bullet">
 * <item><description>The <c>Row Name</c> in the Data Table <b>MUST</b> match the <c>MessageTag</c> exactly for the <c>FindRow</c> lookup logic to succeed.</description></item>
 * <item><description><c>MessageTag</c> must match the Asset Tag of the Gameplay Effect tied to the item being picked up.</description></item>
 * </list>
 * <b>DECLARED CLASSES:</b>
 * <list type="bullet">
 * <item><description><c>X_UI_Controller_HUD</c></description></item>
 * </list>
 * </remarks>
 */
USTRUCT(BlueprintType)
struct FScreenMessageItemPickupRowStructure : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Image;
    
	// Forward declare the class here so we don't need to include the massive View base class header.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<class UX_UI_View_Base> View; 
};

/**
 * <summary>
 * Container struct that maps an <c>FGameplayAttribute</c> to an <c>FGameplayTag</c> and its localized display information.
 * </summary>
 * <remarks>
 * <b>CALLERS:</b>
 * <list type="bullet">
 * <item><description><c>X_UI_Controller_AttributeMenu</c></description></item>
 * <item><description><c>X_AttributeDisplayInfo</c></description></item>
 * <item><description><c>DA_AttributeDisplayInfo</c></description></item>
 * </list>
 * </remarks>
 */
USTRUCT(BlueprintType)
struct FAttributeDisplayInfo
{
	GENERATED_BODY()

	/**
	 * <summary>
	 * The unique <c>FGameplayTag</c> associated with this specific Attribute.
	 * </summary>
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();
	
	/**
	 * <summary>
	 * Core GAS property handle used to identify and read a specific Attribute from the Attribute Set.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>This allows us to map an Attribute's <c>FGameplayTag</c> directly to the actual <c>FGameplayAttribute</c> property without writing hardcoded C++ Switch statements.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayAttribute AttributeGetter;

	/**
	 * <summary>
	 * The localized name of the Attribute.
	 * </summary>
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	/**
	 * <summary>
	 * The localized description explaining what the Attribute does.
	 * </summary>
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	/**
	 * <summary>
	 * The current numeric value of the Attribute, populated dynamically at runtime.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Runtime Only:</b>  This value is not configured in the Data Asset, but is injected by the Controller right before it is broadcast.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AttributeValue = 0.0f;
	
};

/**
 * <summary>
 * Used to consolidate broadcasts of CurrentValue and MaxValue Attributes into a single payload for UI elements to avoid "frame 0" initial View state synchronization issues.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Prevents MaxValue arriving before CurrentValue to avoid SafeDivide calls that result in a percentage value of 0, forcing UI elements to appear empty on "frame 0" of UI initialization.</description></item>
 * </list>
 * </remarks>
 */
USTRUCT(BlueprintType)
struct FX_UIAttributeDisplayCurrentMaxPayload
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "UI|Attributes")
	float CurrentValue = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "UI|Attributes")
	float MaxValue = 0.0f;
};
