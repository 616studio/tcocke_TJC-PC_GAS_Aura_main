// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "X_AttributeDisplayInfo.generated.h"

struct FGameplayTag;
struct FX_AttributeDisplayInfoContainer;

/**
 * <summary>
 * Acts as the master database of UI display data for all project Attributes.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Exists in the Editor as <c>DA_AttributeDisplayInfo</c>.</description></item>
 * </list>
 * </remarks>
 */
UCLASS()
class GAS_AURA_API UX_AttributeDisplayInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	/**
	 * <summary>
	 * Defines the structural layout of the Data Asset as an array of <c>FX_AttributeDisplayInfoContainer</c> mappings.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>The <c>TitleProperty</c> meta tag formats the Editor UI so each element in the array is labeled by its localized <c>AttributeName</c> rather than generic index numbers (ex: [0], [1]).</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "{AttributeName}"))
	TArray<FX_AttributeDisplayInfoContainer> AttributeMappings;
	
#if WITH_EDITOR
	
	/**
	 * <summary>
	 * Native UE5 Editor lifecycle hook executed immediately after a property on this Data Asset is modified in the Details panel.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Invokes <c>UpdateAttributeDisplayDataFromTags</c> to instantly synchronize <c>AttributeName</c> and <c>AttributeDescription</c> fields whenever an <c>AttributeTag</c> selection changes.</description></item>
	 * <item><description>Guarded inside <c>#if WITH_EDITOR</c> to ensure editor-only asset modification routines are completely stripped from cooked standalone builds.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="PropertyChangedEvent">[<c>FPropertyChangedEvent&amp;</c>]: Struct payload containing context data regarding the specific property modified in the Editor Details panel.</param>
	 */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	/**
	 * <summary>
	 * Native UE5 lifecycle hook executed immediately after this Data Asset is loaded into memory by the engine.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Invokes <c>UpdateAttributeDescriptionsFromTags</c> upon asset load in the Editor to ensure any updated C++ Gameplay Tag developer comments (<c>UE_DEFINE_GAMEPLAY_TAG_COMMENT</c>) are automatically synchronized into <c>AttributeDescription</c> fields without requiring manual property edits.</description></item>
	 * <item><description>Guarded inside <c>#if WITH_EDITOR</c> to ensure editor-only asset serialization routines are completely stripped from cooked standalone builds.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void PostLoad() override;
	
protected:
	
private:
	
	/**
	 * <summary>
	 * Queries the <c>UGameplayTagsManager</c> singleton and populates unassigned <c>AttributeName</c> and <c>AttributeDescription</c> fields in <c>AttributeMappings</c> using tag leaf string formatting and C++ Gameplay Tag developer comments.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Iterates through <c>AttributeMappings</c> and extracts developer comments using <c>GetComment()</c> on the tag's underlying <c>FGameplayTagNode</c>.</description></item>
	 * <item><description>Converts tag leaf names (e.g., <c>Attributes.Secondary.ArmorPenetration</c>) into properly spaced, readable titles (e.g., <c>"Armor Penetration"</c>) if <c>AttributeName</c> is empty.</description></item>
	 * <item><description>Automates UI display management, keeping native C++ tag registrations (<c>UE_DEFINE_GAMEPLAY_TAG_COMMENT</c>) as the authoritative source of truth across the project.</description></item>
	 * </list>
	 * </remarks>
	 */
	void UpdateAttributeDisplayDataFromTags();

	/**
	 * <summary>
	 * Helper function that converts a hierarchical Gameplay Tag into a human-readable, space-titled <c>FText</c> string.
	 * </summary>
	 * <param name="Tag">[<c>const FGameplayTag&amp;</c>]: The Gameplay Tag to extract and format a display title from.</param>
	 * <returns>[<c>FText</c>]: The formatted localized display name (e.g., <c>"Armor Penetration"</c> or <c>"Fire Resistance"</c>).</returns>
	 */
	FText GetFormattedAttributeNameFromTag(const FGameplayTag& Tag) const;
	
#endif
};
