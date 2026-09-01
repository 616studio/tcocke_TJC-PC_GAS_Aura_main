// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#include "UI/Controllers/X_UI_Controller_HUD.h"
#include "AbilitySystem/X_AbilitySystemComponent.h"
#include "AbilitySystem/X_AbilitySystemLibrary.h"
#include "AbilitySystem/X_AttributeSet.h"
#include "GAS_Aura/UtilityClasses/X_GameplayTags.h"

void UX_UI_Controller_HUD::BroadcastCurrentModelValues()
{
	const UX_AttributeSet* XAS = Cast<UX_AttributeSet>(AttributeSet);
	if (!ensureMsgf(IsValid(XAS), TEXT("Actor: %s - No valid (XAS) found.  Function: %hs"),
				   *GetName(), __FUNCTION__))
	{
		return;
	}
	
	// For right now, we are only broadcasting these values for the WBP_View_ProgressBar_Globe_Health and WBP_View_ProgressBar_Globe_Mana Views.
	// These WBPs are only interested in Health, MaxHealth, Mana, and MaxMana.
	// They bind to each of these delegates in their own Event Graphs.
	
	FX_UIAttributeDisplayCurrentMaxPayload HealthDisplayInfo;
	HealthDisplayInfo.CurrentValue = XAS->GetHealth();
	HealthDisplayInfo.MaxValue = XAS->GetMaxHealth();
	OnHealthChanged.Broadcast(HealthDisplayInfo);
	
	FX_UIAttributeDisplayCurrentMaxPayload ManaDisplayInfo;
	ManaDisplayInfo.CurrentValue = XAS->GetMana();
	ManaDisplayInfo.MaxValue = XAS->GetMaxMana();
	OnManaChanged.Broadcast(ManaDisplayInfo);
}

void UX_UI_Controller_HUD::BindCallbacksToModelDelegates()
{
	// Prevent duplicate delegate bindings if this is called more than once (ex: respawn, race condition paths).
	if (bCallbacksBound) return;
	
	UX_AttributeSet* XAS = Cast<UX_AttributeSet>(AttributeSet);
	if (!ensureMsgf(IsValid(XAS), TEXT("Actor: %s - No valid (XAS) found.  Function: %hs"),
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
	
	BindToAttributeDelegates(XAS, XASC);
	
	BindToGameplayEffectAssetTagsDelegate(XASC);

	// All delegate bindings succeeded. Check this flag at the top of the function to prevent double-binding.
	bCallbacksBound = true;
}

void UX_UI_Controller_HUD::BindToAttributeDelegates(UX_AttributeSet* XAS, UX_AbilitySystemComponent* XASC)
{
	if (!ensureMsgf(IsValid(XAS), TEXT("Actor: %s - No valid (XAS) found.  Function: %hs"),
	               *GetName(), __FUNCTION__))
	{
		return;
	}

	if (!ensureMsgf(IsValid(XASC), TEXT("Actor: %s - No valid (XASC) found.  Function: %hs"),
	               *GetName(), __FUNCTION__))
	{
		return;
	}
	
	// Always use AddWeakLambda when binding to delegates that might outlive the UI widget.
	
	// XAS is a local function parameter residing on the stack. The capture clause [this] only grants access to member variables belonging to this. 
	// It does not capture function parameters from the outer scope, triggering a compiler error.
	// AddWeakLambda guards 'this'. Downcasting the generic member 'this->AttributeSet' (declared on X_UI_Controller_Base) to 'Lambda_XAS' inside the closure resolves stack capture errors.
	
	// Health Attribute Update
	XASC->GetGameplayAttributeValueChangeDelegate(XAS->GetHealthAttribute()).AddWeakLambda(this, [this](const FOnAttributeChangeData& Data)
	{
		// Downcast the persistent GC-tracked base member pointer stored on UX_UI_Controller_Base.
		UX_AttributeSet* Lambda_XAS = Cast<UX_AttributeSet>(this->AttributeSet);
		if (!IsValid(Lambda_XAS)) return;
		
		FX_UIAttributeDisplayCurrentMaxPayload DisplayInfo;
		DisplayInfo.CurrentValue = Data.NewValue;
		DisplayInfo.MaxValue = Lambda_XAS->GetMaxHealth();
		
		OnHealthChanged.Broadcast(DisplayInfo);
	});
	
	// MaxHealth Attribute Update
	XASC->GetGameplayAttributeValueChangeDelegate(XAS->GetMaxHealthAttribute()).AddWeakLambda(this, [this](const FOnAttributeChangeData& Data)
	{
		// Downcast the persistent GC-tracked base member pointer stored on UX_UI_Controller_Base.
		UX_AttributeSet* Lambda_XAS = Cast<UX_AttributeSet>(this->AttributeSet);
		if (!IsValid(Lambda_XAS)) return;
			
		FX_UIAttributeDisplayCurrentMaxPayload DisplayInfo;
		DisplayInfo.CurrentValue = Lambda_XAS->GetHealth();
		DisplayInfo.MaxValue = Data.NewValue;
			
		OnHealthChanged.Broadcast(DisplayInfo);
	});
	
	// Mana Attribute Update
	XASC->GetGameplayAttributeValueChangeDelegate(XAS->GetManaAttribute()).AddWeakLambda(this, [this](const FOnAttributeChangeData& Data)
	{
		// Downcast the persistent GC-tracked base member pointer stored on UX_UI_Controller_Base.
		UX_AttributeSet* Lambda_XAS = Cast<UX_AttributeSet>(this->AttributeSet);
		if (!IsValid(Lambda_XAS)) return;
			
		FX_UIAttributeDisplayCurrentMaxPayload DisplayInfo;
		DisplayInfo.CurrentValue = Data.NewValue;
		DisplayInfo.MaxValue = Lambda_XAS->GetMaxMana();
			
		OnManaChanged.Broadcast(DisplayInfo);
	});
	
	// MaxMana Attribute Update
	XASC->GetGameplayAttributeValueChangeDelegate(XAS->GetMaxManaAttribute()).AddWeakLambda(this, [this](const FOnAttributeChangeData& Data)
	{
		// Downcast the persistent GC-tracked base member pointer stored on UX_UI_Controller_Base.
		UX_AttributeSet* Lambda_XAS = Cast<UX_AttributeSet>(this->AttributeSet);
		if (!IsValid(Lambda_XAS)) return;
			
		FX_UIAttributeDisplayCurrentMaxPayload DisplayInfo;
		DisplayInfo.CurrentValue = Lambda_XAS->GetMana();
		DisplayInfo.MaxValue = Data.NewValue;
			
		OnManaChanged.Broadcast(DisplayInfo);
	});
}

void UX_UI_Controller_HUD::BindToGameplayEffectAssetTagsDelegate(UX_AbilitySystemComponent* XASC)
{
	if (!ensureMsgf(IsValid(XASC), TEXT("Actor: %s - No valid (XASC) found.  Function: %hs"),
	               *GetName(), __FUNCTION__))
	{
		return;
	}

	if (!ensureMsgf(
		IsValid(DataTableScreenMessageItemPickup), TEXT("Actor: %s - Missing Editor assigned variable (%s).  Assign in Blueprint Editor: Custom - UI.  Function: %hs"),
		*GetName(), *GET_MEMBER_NAME_CHECKED(ThisClass, DataTableScreenMessageItemPickup).ToString(), __FUNCTION__))
	{
		return;
	}
	
		// Always use AddWeakLambda when binding to delegates that might outlive the UI widget.
		XASC->OnIncomingGameplayEffectAssetTags.AddWeakLambda(this,
		[this](const FGameplayTagContainer& GameplayEffectAssetTags)
		{
			// Deferred safety: Ensure DT reference wasn't cleared at runtime
			if (!IsValid(DataTableScreenMessageItemPickup)) return;
			
			// Iterate through every tag belonging to the Gameplay Effect that was just applied to the XASC.
			for (const FGameplayTag& Tag : GameplayEffectAssetTags)
			{
				// If the incoming tag belongs to our custom Gameplay Tag "UI.Message.Item" hierarchy...
				if (Tag.MatchesTag(XGameplayTags::UI_Message_Item))
				{
					// Use our Ability System Library to find the matching row in the Data Table.
					const FScreenMessageItemPickupRowStructure* Row = UX_AbilitySystemLibrary::GetDataTableRowByTag<FScreenMessageItemPickupRowStructure>(DataTableScreenMessageItemPickup, Tag);
							
					// If a matching row was successfully found, broadcast that row's data.
					if (Row)
					{
						OnScreenMessageItemPickup.Broadcast(*Row);
					}
					else
					{
						// Catch row name mismatches between native tags and Data Table rows.
						UE_LOG(LogTemp, Warning, TEXT("%s: Received Asset Tag [%s], but no matching row was found in (%s)! Ensure Data Table Row Name matches '%s' exactly."), 
							*GetName(), *Tag.ToString(), *GET_MEMBER_NAME_CHECKED(ThisClass, DataTableScreenMessageItemPickup).ToString(), *Tag.ToString());
					}
				}
			}
		}
	);
}
