// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "X_CursorHighlightInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UX_CursorHighlightInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * <summary>
 * Provides implementation details for any actor that needs to be highlightable by the player's mouse cursor.
 * </summary>
*/
class GAS_AURA_API IX_CursorHighlightInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void AddHighlightToActor() = 0;
	virtual void RemoveHighlightFromActor() = 0;

	// Value of 250.0f is for the color Red in order to work with the custom PP_Highlight material.
	// Will NOT work unless you enable it in Project Settings - Engine - Rendering - Post Processing:  Custom Depth-Stencil Pass -> Enabled with Stencil
	virtual float GetCustomStencilDepthValue() const = 0;
	
};
