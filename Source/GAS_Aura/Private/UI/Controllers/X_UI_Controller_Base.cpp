// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#include "UI/Controllers/X_UI_Controller_Base.h"
#include "GAS_Aura/UtilityClasses/X_CustomDataTypes.h"

void UX_UI_Controller_Base::AssignModelsToController(const FModelsPayload& Models)
{
	PlayerController = Models.PlayerController;
	PlayerState = Models.PlayerState;
	AbilitySystemComponent = Models.AbilitySystemComponent;
	AttributeSet = Models.AttributeSet;
}

void UX_UI_Controller_Base::BindCallbacksToModelDelegates()
{	
}

void UX_UI_Controller_Base::BroadcastCurrentModelValues()
{	
}
