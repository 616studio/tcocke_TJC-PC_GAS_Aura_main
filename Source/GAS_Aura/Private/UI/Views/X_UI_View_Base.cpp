// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))


#include "UI/Views/X_UI_View_Base.h"

void UX_UI_View_Base::AssignControllerToView(UObject* ControllerToAssign)
{
	CurrentlyAssignedController = ControllerToAssign;

	// Notify Blueprint subclasses that the Controller is safely cached and ready for data binding.
	ControllerHasBeenAssigned();
}