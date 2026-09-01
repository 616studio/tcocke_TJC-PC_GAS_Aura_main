// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))


#include "UI/HUD/X_HUD.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "UI/Views/X_UI_View_Base.h"
#include "UI/Controllers/X_UI_Controller_Base.h"
#include "UI/Controllers/X_UI_Controller_AttributeMenu.h"
#include "UI/Controllers/X_UI_Controller_HUD.h"

UX_UI_Controller_HUD* AX_HUD::GetView_HUDController(const FModelsPayload& Models)
{
	// Lazy Init: instantiates the Controller from the Editor assigned Blueprint class if it has not yet been cached.
	if (View_HUDController == nullptr)
	{
		if (!ensureMsgf(IsValid(View_HUDControllerClass), TEXT("Actor: %s - Missing Editor assigned variable (%s).  Function: %hs"),
				   *GetName(),
				   *GET_MEMBER_NAME_CHECKED(ThisClass, View_HUDControllerClass).ToString(),
				   __FUNCTION__))
		{
			return nullptr;
		}
		
		View_HUDController = NewObject<UX_UI_Controller_HUD>(this, View_HUDControllerClass);		
	}
	
	// NOTE: validation of the Models payload is the responsibility of the Controller, not this class.
	View_HUDController->AssignModelsToController(Models);
	View_HUDController->BindCallbacksToModelDelegates();
	
	return View_HUDController;
	
}

UX_UI_Controller_AttributeMenu* AX_HUD::GetView_AttributeMenuController(const FModelsPayload& Models)
{
	// Lazy Init: instantiates the Controller from the Editor assigned Blueprint class if it has not yet been cached.
	if (View_AttributeMenuController == nullptr)
	{
		if (!ensureMsgf(IsValid(View_AttributeMenuControllerClass), TEXT("Actor: %s - Missing Editor assigned variable (%s).  Function: %hs"),
					   *GetName(),
					   *GET_MEMBER_NAME_CHECKED(ThisClass, View_AttributeMenuControllerClass).ToString(),
					   __FUNCTION__))
		{
			return nullptr;
		}
		
		View_AttributeMenuController = NewObject<UX_UI_Controller_AttributeMenu>(this, View_AttributeMenuControllerClass);		
	}
	
	// NOTE: validation of the Models payload is the responsibility of the Controller, not this class.
	View_AttributeMenuController->AssignModelsToController(Models);
	View_AttributeMenuController->BindCallbacksToModelDelegates();
	
	return View_AttributeMenuController;
}

void AX_HUD::InitView_HUD(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// Defers initialization until all Models have fully replicated to the Client.
	if (!IsValid(PC) || !IsValid(PS) || !IsValid(ASC) || !IsValid(AS))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor: %s.  Missing Model dependency due to network replication delay.  Function: %hs"), 
				*GetName(), 
				__FUNCTION__);
		return;
	}
	
	if (!IsValid(View_HUDClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor: %s.  Missing Editor assigned variable (%s).  Function: %hs"),
		       *GetName(),
		       *GET_MEMBER_NAME_CHECKED(ThisClass, View_HUDClass).ToString(),
		       __FUNCTION__);
		
		return;		
	}

	if (View_HUD == nullptr)
	{
		// Binds View_HUD to the specified PlayerController to set its OwningPlayer, guaranteeing correct input routing and local screen allocation.
		View_HUD = CreateWidget<UX_UI_View_Base>(PC, View_HUDClass);
	}

	if (!IsValid(View_HUD))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor: %s.  No valid (%s) found.  Function: %hs"),
		       *GetName(),
		       *GET_MEMBER_NAME_CHECKED(ThisClass, View_HUD).ToString(),
		       __FUNCTION__);
		
		return;
		
	}

	// Construct a Model payload to retrieve a fully initialized Controller ready to be assigned to View_HUD.	
	const FModelsPayload Models(PC, PS, ASC, AS);
	View_HUDController = GetView_HUDController(Models);

	if (!IsValid(View_HUDController))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor: %s.  No valid (%s) found.  Function: %hs"),
		       *GetName(),
		       *GET_MEMBER_NAME_CHECKED(ThisClass, View_HUDController).ToString(),
		       __FUNCTION__);
		
		return;
		
	}

	// Triggers the "ControllerHasBeenAssigned" Blueprint event, allowing the View_HUD to distribute its assigned Controller to any of its embedded child Views.
	View_HUD->AssignControllerToView(View_HUDController);

	// Broadcast initial Model data to View_HUD to prevent empty UI states.
	View_HUDController->BroadcastCurrentModelValues();
					
	if (!View_HUD->IsInViewport())
	{
		View_HUD->AddToViewport();
	}	
}