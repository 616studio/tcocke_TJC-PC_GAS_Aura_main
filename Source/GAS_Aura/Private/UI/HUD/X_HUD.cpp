// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))


#include "UI/HUD/X_HUD.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "UI/Views/X_UI_View_Base.h"
#include "UI/Controllers/X_UI_Controller_Base.h"

void AX_HUD::InitHUD(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// Defers initialization until all Models have fully replicated to the Client.
	if (!IsValid(PC) || !IsValid(PS) || !IsValid(ASC) || !IsValid(AS))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor: %s.  Missing Model dependency due to network replication delay.  Function: %hs"), 
				*GetName(), 
				__FUNCTION__);
		return;
	}
	
	// Store Model context payload locally for lazy Controller instantiation.
	CurrentModelsPayload = FModelsPayload(PC, PS, ASC, AS);
	
	// Cascade updated Models and re-bind delegates for all ALREADY-instantiated controllers (handles Pawn respawns/re-possession).
	for (const TPair<TSubclassOf<UX_UI_Controller_Base>, TObjectPtr<UX_UI_Controller_Base>>& KVP : ControllerRegistry)
	{
		if (UX_UI_Controller_Base* Controller = KVP.Value)
		{
			Controller->AssignModelsToController(CurrentModelsPayload);
			Controller->BindCallbacksToModelDelegates();
		}
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
	
	// Broadcast initialization signal down the View tree (Child sub-widgets pull Controllers via UX_AbilitySystemLibrary).
	if (View_HUD->Implements<UX_UI_ViewInterface>())
	{
		IX_UI_ViewInterface::Execute_PerformInitialization(View_HUD);
	}
	
	// Render top-level layout container to Viewport.
	View_HUD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (!View_HUD->IsInViewport())
	{
		View_HUD->AddToViewport();
	}
}

UX_UI_Controller_Base* AX_HUD::GetOrCreateController_Internal(TSubclassOf<UX_UI_Controller_Base> ControllerClass)
{
	if (!IsValid(ControllerClass)) return nullptr;

	// Return cached instance if already initialized.
	if (TObjectPtr<UX_UI_Controller_Base>* ExistingController = ControllerRegistry.Find(ControllerClass))
	{
		return *ExistingController;
	}

	// Instantiate new Blueprint subclass and assign Models.
	UX_UI_Controller_Base* NewController = NewObject<UX_UI_Controller_Base>(this, ControllerClass);
	NewController->AssignModelsToController(CurrentModelsPayload);
	NewController->BindCallbacksToModelDelegates();

	// Cache Controller instance in TMap for Garbage Collection tracking.
	ControllerRegistry.Add(ControllerClass, NewController);
	return NewController;
}
