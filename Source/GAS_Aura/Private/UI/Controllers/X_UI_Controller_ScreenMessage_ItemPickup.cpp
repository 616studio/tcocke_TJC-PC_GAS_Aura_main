// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#include "UI/Controllers/X_UI_Controller_ScreenMessage_ItemPickup.h"
#include "AbilitySystem/X_AbilitySystemComponent.h"
#include "AbilitySystem/X_AbilitySystemLibrary.h"
#include "AbilitySystem/X_AttributeSet.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GAS_Aura/UtilityClasses/X_GameplayTags.h"

void UX_UI_Controller_ScreenMessage_ItemPickup::BindCallbacksToModelDelegates()
{
	// Prevent duplicate delegate bindings if this is called more than once (ex: respawn, race condition paths).
	if (bCallbacksBound) return;
	
	UX_AbilitySystemComponent* XASC = Cast<UX_AbilitySystemComponent>(AbilitySystemComponent);
	if (!ensureMsgf(IsValid(XASC), TEXT("Actor: %s - No valid (XASC) found.  Function: %hs"),
				   *GetName(),
				   __FUNCTION__))
	{
		return;
	}
	
	BindToGameplayEffectAssetTagsDelegate(XASC);

	// All delegate bindings succeeded. 
	// Check this flag at the top of the function to prevent double-binding.
	bCallbacksBound = true;
}

void UX_UI_Controller_ScreenMessage_ItemPickup::PreloadScreenMessageDataTableAssets()
{
	if (!IsValid(DataTableScreenMessageItemPickup)) return;

	// Temporary container used to accumulate soft asset paths extracted from each row of the Data Table.
	TArray<FSoftObjectPath> AssetsToLoad;

	const TArray<FName> RowNames = DataTableScreenMessageItemPickup->GetRowNames();

	for (const FName& RowName : RowNames)
	{
		if (const FScreenMessageItemPickupRowStructure* Row = DataTableScreenMessageItemPickup->FindRow<FScreenMessageItemPickupRowStructure>(RowName, TEXT("PreloadScreenMessageDataTableAssets")))
		{
			if (!Row->Image.IsNull())
			{
				AssetsToLoad.AddUnique(Row->Image.ToSoftObjectPath());
			}
			if (!Row->View.IsNull())
			{
				AssetsToLoad.AddUnique(Row->View.ToSoftObjectPath());
			}
		}
	}

	if (!AssetsToLoad.IsEmpty())
	{
		// Request the global Engine AssetManager's StreamableManager to load all accumulated paths asynchronously in the background.
		// We pass a UObject delegate pointing to OnScreenMessageAssetsPreloaded and forward AssetsToLoad as a parameter payload to the completion callback.
		UAssetManager::GetStreamableManager().RequestAsyncLoad(AssetsToLoad, FStreamableDelegate::CreateUObject(this, &ThisClass::OnScreenMessageAssetsPreloaded, AssetsToLoad));
	}
}

void UX_UI_Controller_ScreenMessage_ItemPickup::OnScreenMessageAssetsPreloaded(TArray<FSoftObjectPath> PreloadedPaths)
{
	// Iterate through every soft object path that was handed back to us upon completion of the asynchronous stream request.
	for (const FSoftObjectPath& Path : PreloadedPaths)
	{
		// Attempt to resolve the soft path string into a hard, in-memory UObject pointer (guaranteed non-blocking since AsyncLoad just completed).
		if (UObject* LoadedAsset = Path.ResolveObject())
		{
			// Add the resolved UObject pointer into our GC-protected member array (PreloadedScreenMessageAssets).
			// By decorating PreloadedScreenMessageAssets with UPROPERTY() in the header, Unreal's Garbage Collector is prohibited from purging these UI widgets and textures from RAM while this Controller lives.
			PreloadedScreenMessageAssets.AddUnique(LoadedAsset);
		}
	}
}

void UX_UI_Controller_ScreenMessage_ItemPickup::BindToGameplayEffectAssetTagsDelegate(UX_AbilitySystemComponent* XASC)
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
	
	// Warm up / stream-preload all soft assets in the Data Table.
	PreloadScreenMessageDataTableAssets();
	
	// Always use AddWeakLambda when binding to delegates that might outlive the UI widget.
	XASC->OnIncomingGameplayEffectAssetTags.AddWeakLambda(this,
	[this](const FGameplayTagContainer& GameplayEffectAssetTags)
	{
		// Ensure Data Table reference wasn't cleared at runtime.
		if (!IsValid(DataTableScreenMessageItemPickup)) return;
		
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
