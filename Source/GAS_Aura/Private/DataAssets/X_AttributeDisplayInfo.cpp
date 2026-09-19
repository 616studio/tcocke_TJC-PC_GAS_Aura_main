// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#include "DataAssets/X_AttributeDisplayInfo.h"

#include "GameplayTagsManager.h"
#include "GAS_Aura/UtilityClasses/X_CustomDataTypes.h"

#if WITH_EDITOR
void UX_AttributeDisplayInfo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	UpdateAttributeDisplayDataFromTags();
}

void UX_AttributeDisplayInfo::PostLoad()
{
	Super::PostLoad();

	UpdateAttributeDisplayDataFromTags();
}

void UX_AttributeDisplayInfo::UpdateAttributeDisplayDataFromTags()
{
	const UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

	for (FX_AttributeDisplayInfoContainer& Mapping : AttributeMappings)
	{
		if (Mapping.AttributeTag.IsValid())
		{
			// Auto-populate AttributeName from C++ Gameplay Tag hierarchy.
			Mapping.AttributeName = GetFormattedAttributeNameFromTag(Mapping.AttributeTag);

			// Auto-populate AttributeDescription from C++ Gameplay Tag DevComment.
			const TSharedPtr<FGameplayTagNode> TagNode = TagsManager.FindTagNode(Mapping.AttributeTag);
			Mapping.AttributeDescription = FText::FromString(TagNode->GetDevComment());
		}
		else
		{
			Mapping.AttributeName = FText::GetEmpty();
			Mapping.AttributeDescription = FText::GetEmpty();
		}
	}
}

FText UX_AttributeDisplayInfo::GetFormattedAttributeNameFromTag(const FGameplayTag& Tag) const
{
	if (!Tag.IsValid())
	{
		return FText::GetEmpty();
	}

	const FString TagName = Tag.ToString();
	FString LeafName;

	// Extract the leaf segment after the last dot (ex: "ArmorPenetration").
	if (!TagName.Split(TEXT("."), nullptr, &LeafName, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
	{
		LeafName = TagName;
	}

	// Insert spaces before uppercase letters (PascalCase to Spaced Title Case).
	FString SpacedName;
	for (int32 i = 0; i < LeafName.Len(); ++i)
	{
		const TCHAR Char = LeafName[i];
		if (i > 0 && FChar::IsUpper(Char) && !FChar::IsUpper(LeafName[i - 1]))
		{
			SpacedName.AppendChar(' ');
		}
		SpacedName.AppendChar(Char);
	}

	return FText::FromString(SpacedName);
}
#endif