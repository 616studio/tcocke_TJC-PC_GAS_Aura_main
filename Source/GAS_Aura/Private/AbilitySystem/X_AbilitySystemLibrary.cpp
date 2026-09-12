// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#include "AbilitySystem/X_AbilitySystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/X_UI_ViewInterface.h"
#include "UI/HUD/X_HUD.h"
#include "UI/Controllers/X_UI_Controller_Base.h"

class AX_PlayerState;

#pragma region UI

UX_UI_Controller_Base* UX_AbilitySystemLibrary::GetUIController(UObject* WorldContextObject,
	TSubclassOf<UX_UI_Controller_Base> ControllerClassToAssign)
{
	if (!ensureMsgf(IsValid(WorldContextObject), TEXT("No valid (WorldContextObject) found.  Function: %hs"), __FUNCTION__))
	{
		return nullptr;
	}

	if (!ensureMsgf(IsValid(ControllerClassToAssign), TEXT("Missing Editor assigned variable (ControllerClassToAssign).  Assign in Blueprint.  Function: %hs"), __FUNCTION__))
	{
		return nullptr;
	}

	if (GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (APlayerController* PC = GetPlayerControllerFromWorldContextObject(WorldContextObject))
		{
			if (AX_HUD* HUD = Cast<AX_HUD>(PC->GetHUD()))
			{
				return HUD->GetOrCreateController(ControllerClassToAssign);
			}
		}
	}

	return nullptr;
}

void UX_AbilitySystemLibrary::InitializeChildViews(UWidget* RootWidget)
{
	if (!IsValid(RootWidget)) return;

	// If RootWidget implements IX_UI_ViewInterface, trigger its PerformInitialization execution.
	if (RootWidget->Implements<UX_UI_ViewInterface>())
	{
		IX_UI_ViewInterface::Execute_PerformInitialization(RootWidget);
	}

	// If RootWidget is a container (UCanvasPanel, UWrapBox, UScrollBox, UVerticalBox, etc.), traverse its children.
	if (const UPanelWidget* PanelWidget = Cast<UPanelWidget>(RootWidget))
	{
		const int32 NumChildren = PanelWidget->GetChildrenCount();
		for (int32 i = 0; i < NumChildren; ++i)
		{
			if (UWidget* ChildWidget = PanelWidget->GetChildAt(i))
			{
				InitializeChildViews(ChildWidget);
			}
		}
	}
}

#pragma endregion UI

#pragma region Utility

APlayerController* UX_AbilitySystemLibrary::GetPlayerControllerFromWorldContextObject(UObject* WorldContextObject)
{
	if (APlayerController* PC = Cast<APlayerController>(WorldContextObject))
	{
		return PC;
	}

	if (const APawn* Pawn = Cast<APawn>(WorldContextObject))
	{
		return Cast<APlayerController>(Pawn->GetController());
	}

	if (const APlayerState* PS = Cast<APlayerState>(WorldContextObject))
	{
		return PS->GetPlayerController();
	}

	if (const UUserWidget* Widget = Cast<UUserWidget>(WorldContextObject))
	{
		return Widget->GetOwningPlayer();
	}

	// Fallback: return nullptr instead of "Player 0".
	// Returning "Player 0" would return the WRONG player's controller in multiplayer, resulting in the wrong HUD, ASC, and Attributes being used. 
	return nullptr;
}

#pragma endregion Utility