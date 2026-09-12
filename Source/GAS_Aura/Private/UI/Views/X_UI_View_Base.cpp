// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))


#include "UI/Views/X_UI_View_Base.h"

void UX_UI_View_Base::PerformInitialization_Implementation()
{
	ReadyToInitialize();
}

void UX_UI_View_Base::NativeDestruct()
{
	Super::NativeDestruct();
	
	bIsInitialized = false;
}
