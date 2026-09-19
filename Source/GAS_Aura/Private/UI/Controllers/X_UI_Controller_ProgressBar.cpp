// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))


#include "UI/Controllers/X_UI_Controller_ProgressBar.h"
#include "AbilitySystem/X_AbilitySystemComponent.h"
#include "AbilitySystem/X_AttributeSet.h"

void UX_UI_Controller_ProgressBar::BroadcastCurrentModelValues()
{
	// Check to make sure the Attributes we want to capture were assigned in the Editor.
	if (!ensureMsgf(CurrentValueAttribute.IsValid(), TEXT("Actor: %s - No valid (CurrentValueAttribute) found.  Function: %hs"),
	               *GetName(), __FUNCTION__))
	{
		return;
	}

	if (!ensureMsgf(MaxValueAttribute.IsValid(), TEXT("Actor: %s - No valid (MaxValueAttribute) found.  Function: %hs"),
	               *GetName(), __FUNCTION__))
	{
		return;
	}
	
	UX_AbilitySystemComponent* XASC = Cast<UX_AbilitySystemComponent>(AbilitySystemComponent);
	if (!ensureMsgf(IsValid(XASC), TEXT("Actor: %s - No valid (XASC) found.  Function: %hs"),
				   *GetName(),
				   __FUNCTION__))
	{
		return;
	}
	
	AttributeInfo.CurrentValue = XASC->GetNumericAttribute(CurrentValueAttribute);
	AttributeInfo.MaxValue = XASC->GetNumericAttribute(MaxValueAttribute);
	OnAttributeValueChangedDelegate.Broadcast(AttributeInfo);	
}

void UX_UI_Controller_ProgressBar::BindCallbacksToModelDelegates()
{
	// Prevent duplicate delegate bindings if this is called more than once (ex: respawn, race condition paths).
	if (bCallbacksBound) return;
	
	// Check to make sure the Attributes we want to capture were assigned in the Editor.
	if (!ensureMsgf(CurrentValueAttribute.IsValid(), TEXT("Actor: %s - No valid (CurrentValueAttribute) found.  Function: %hs"),
				   *GetName(), __FUNCTION__))
	{
		return;
	}

	if (!ensureMsgf(MaxValueAttribute.IsValid(), TEXT("Actor: %s - No valid (MaxValueAttribute) found.  Function: %hs"),
				   *GetName(), __FUNCTION__))
	{
		return;
	}
	
	UX_AbilitySystemComponent* XASC = Cast<UX_AbilitySystemComponent>(AbilitySystemComponent);
	if (!ensureMsgf(IsValid(XASC), TEXT("Actor: %s - No valid (XASC) found.  Function: %hs"),
				   *GetName(),
				   __FUNCTION__))
	{
		return;
	}
	
	// Bind Current value.
	XASC->GetGameplayAttributeValueChangeDelegate(CurrentValueAttribute).AddWeakLambda(this,
			
		[this, XASC](const FOnAttributeChangeData& Data)
		{
			if (!IsValid(XASC)) return;
			
			AttributeInfo.CurrentValue = Data.NewValue;
			AttributeInfo.MaxValue = XASC->GetNumericAttribute(MaxValueAttribute);
			
			OnAttributeValueChangedDelegate.Broadcast(AttributeInfo);
		}
	);
	
	// Bind Max value.
	XASC->GetGameplayAttributeValueChangeDelegate(MaxValueAttribute).AddWeakLambda(this,
			
		[this, XASC](const FOnAttributeChangeData& Data)
		{
			if (!IsValid(XASC)) return;
			
			AttributeInfo.CurrentValue = XASC->GetNumericAttribute(CurrentValueAttribute);
			AttributeInfo.MaxValue = Data.NewValue;
			
			OnAttributeValueChangedDelegate.Broadcast(AttributeInfo);
		}
	);
	
	// All delegate bindings succeeded. 
	// Check this flag at the top of the function to prevent double-binding.
	bCallbacksBound = true;
}
