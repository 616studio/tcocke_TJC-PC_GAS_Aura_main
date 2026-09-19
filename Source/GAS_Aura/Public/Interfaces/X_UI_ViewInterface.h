// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "X_UI_ViewInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UX_UI_ViewInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * <summary>
 * Allows external callers to communicate to Views without performing hard casts to the View's class type.
 * </summary>
 */
class GAS_AURA_API IX_UI_ViewInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	/**
	 * <summary>
	 * Communicate to a View it is safe to start its initialization process.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Can also be used by external callers to force a View to refresh its data.</description></item>
	 * <item><description>Triggers a View's internal Blueprint Event <c>ReadyToInitialize</c>.</description></item>
	 * </list>
	 * </remarks>
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PerformInitialization();
};
