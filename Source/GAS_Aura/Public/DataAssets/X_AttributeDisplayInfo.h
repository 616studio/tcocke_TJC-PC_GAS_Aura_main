// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "X_AttributeDisplayInfo.generated.h"

struct FAttributeDisplayInfo;

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
	 * Defines the structural layout of the Data Asset as an array of <c>FAttributeDisplayInfo</c> mappings.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>The <c>TitleProperty</c> meta tag formats the Editor UI so each element in the array is labeled by its localized <c>AttributeName</c> rather than generic index numbers (ex: [0], [1]).</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "{AttributeName}"))
	TArray<FAttributeDisplayInfo> AttributeMappings;
	
};
