// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#include "UI/Controllers/X_UI_Controller_AttributeDisplayInfo.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/X_AttributeSet.h"
#include "DataAssets/X_AttributeDisplayInfo.h"

void UX_UI_Controller_AttributeDisplayInfo::BroadcastCurrentModelValues()
{
	// Leave if ANY of these are invalid: the ASC, the AttributeSet, the Data Asset, or if the Data Asset has no entries.
	if (!IsValid(AbilitySystemComponent) || !IsValid(AttributeSet) || !IsValid(AttributeDisplayInfoDataAsset) || AttributeDisplayInfoDataAsset->AttributeMappings.IsEmpty()) return;

	// ARCHITECTURE RULE: UX_AttributeSet Exclusive.
	// The FGameplayAttributes in our Data Asset are declared inside UX_AttributeSet.
	// Attempting to evaluate these pointers on a different AttributeSet class will read invalid memory and risk a fatal crash.
	if (!Cast<UX_AttributeSet>(AttributeSet)) return;
	
	// Iterates through the Data Asset to broadcast initial Attribute states to listening Views of this Controller.
	for (FAttributeDisplayInfo& Info : AttributeDisplayInfoDataAsset->AttributeMappings)
	{
		BroadcastAttributeDisplayInfo(Info);
	}
}

void UX_UI_Controller_AttributeDisplayInfo::BindCallbacksToModelDelegates()
{
	// Prevent duplicate delegate bindings if this is called more than once (ex: respawn, race condition paths).
	if (bCallbacksBound) return;
	
	// Leave if ANY of these are invalid: the AbilitySystemComponent, the AttributeSet, the Data Asset, or if the Data Asset has no entries.
	if (!IsValid(AbilitySystemComponent) || !IsValid(AttributeSet) || !IsValid(AttributeDisplayInfoDataAsset) || AttributeDisplayInfoDataAsset->AttributeMappings.IsEmpty()) return;
	
	// ARCHITECTURE RULE: UX_AttributeSet Exclusive.
	// The FGameplayAttributes in our Data Asset are declared inside UX_AttributeSet.
	// Attempting to evaluate these pointers on a different AttributeSet class will read invalid memory and risk a fatal crash.
	if (!Cast<UX_AttributeSet>(AttributeSet)) return;

	// Loop through each FAttributeDisplayInfo entry (Info) in AttributeDisplayInfoDataAsset.
	// Pass in each Attribute to bind to the native GAS engine delegate GetGameplayAttributeValueChangeDelegate (which ultimately leads to GameplayEffectTypes::FOnAttributeChangeData).
	// When a change in an Attribute is detected, pass in the Info struct of the Attribute that changed into helper function BroadcastAttributeDisplayInfo.
	for (FAttributeDisplayInfo& Info : AttributeDisplayInfoDataAsset->AttributeMappings)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Info.Attribute).AddWeakLambda(this,
			
			// ARCHITECTURE NOTE: BINDING DELEGATES USING LAMBDAS
			// 'Info' MUST be captured by value: [this, Info].
			// Capturing by reference ([&Info]) stores a memory pointer pointing directly into the TArray (AttributeMappings) heap buffer. 
			// Delegates fire unpredictably, which means the stored memory address might be deleted or moved by the engine before the lambda runs, causing a fatal crash.
			// Capturing by value forces the lambda to make its own personal, permanent copy of the payload to keep it safe.
			[this, Info](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeDisplayInfo(Info);
			}
		);
	}

	// All delegate bindings succeeded. 
	// Check this flag at the top of the function to prevent double-binding.
	bCallbacksBound = true;
}

void UX_UI_Controller_AttributeDisplayInfo::BroadcastAttributeDisplayInfo(FAttributeDisplayInfo Info) const
{
	// By passing our AttributeSet into GetNumericValue, the native GAS engine dynamically looks up whatever float value is currently stored there.
	Info.AttributeValue = Info.Attribute.GetNumericValue(AttributeSet);
	
	// Broadcast the fully populated payload which now contains the latest value of the Attribute.
	AttributeDisplayInfoDelegate.Broadcast(Info);
}
