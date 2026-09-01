// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "X_UI_DataTypes.h"
#include "GameplayTagContainer.h"
#include "X_UI_Delegates.generated.h"

#pragma region UHT Hack

// Adding this dummy struct tricks the Unreal Header Tool into indexing this file so our DYNAMIC delegates work!
USTRUCT()
struct FX_UI_Delegates_DummyStruct
{
	GENERATED_BODY()
};

#pragma endregion UHT Hack

/**
 * <summary>
 * Used to broadcast the Asset Tags extracted from a Gameplay Effect applied to (incoming) or by (outgoing) the <c>X_AbilitySystemComponent</c>.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description><b>UI Decoupling:</b> Acts as the primary UI communication bridge between the <c>X_AbilitySystemComponent</c> Model and the Controllers.</description></item>
 * <item><description><b>Event-Driven Feedback:</b> Allows UI elements to react to item pickups by the Player purely by listening for specific tags, completely decoupling the View from the actual Gameplay Effect calculation logic.</description></item>
 * </list>
 * <b>DECLARED CLASSES:</b>
 * <list type="bullet">
 * <item><description><c>X_AbilitySystemComponent</c></description></item>
 * </list>
 * </remarks>
 * <param name="AssetTags">The container of Gameplay Tags extracted directly from the applied Gameplay Effect.</param>
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FX_GameplayEffectAssetTagsSignature, const FGameplayTagContainer& /* AssetTags */);

/**
 * <summary>
 * Broadcasts a row of data from <c>DT_ScreenMessage_ItemPickup</c> (Message, Image, and View) where "RowName" matches the supplied Gameplay Tag. 
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description><b>Event-Driven UI:</b> Allows the UI to visually react to item pickups by the Player purely driven by incoming Gameplay Tags.</description></item>
 * </list>
 * <b>DECLARED CLASSES:</b>
 * <list type="bullet">
 * <item><description><c>X_UI_Controller_HUD</c></description></item>
 * </list>
 * </remarks>
 * <param name="Row">The <c>FScreenMessageItemPickupRowStructure</c> row structure containing the Message, image, and View required to display the message on the screen.</param>
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FX_ScreenMessageItemPickupSignature, FScreenMessageItemPickupRowStructure, Row);

/**
 * <summary>
 * Used to broadcast a Gameplay Attribute UI specific payload to a View.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description><b>Data-Driven UI:</b> Enables a scalable UI architecture. Instead of creating separate delegates for Strength, Intelligence, Armor, etc., this single delegate broadcasts any attribute that changes.</description></item>
 * <item><description><b>MVVM Pipeline:</b> The Controller does the heavy lifting: it grabs the raw float from the Model, packages it into the <c>FAttributeDisplayInfo</c> struct alongside the localized text from the Data Asset, and pushes the finalized payload.</description></item>
 * </list>
 * <b>CALLERS:</b>
 * <list type="bullet">
 * <item><description><c>X_UI_Controller_AttributeMenu</c></description></item>
 * </list>
 * </remarks>
 * <param name="AttributeDisplayInfo">The packaged struct payload containing the Gameplay Attribute's Tag, localized display name, description, and current numeric value.</param>
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FX_AttributeDisplayInfoSignature, const FAttributeDisplayInfo&, AttributeDisplayInfo);

/**
 * <summary>
 * Broadcasts a consolidated Attribute payload for the Current and Max Values for UI elements to properly display the correct percentage values on "frame 0" of their initialization.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description><b>MVC Pipeline:</b> Acts as the final step in the Model-View-Controller data flow. The Controller intercepts raw ASC attribute changes and rebroadcasts them through this delegate.</description></item>
 * </list>
 * <b>CALLERS:</b>
 * <list type="bullet">
 * <item><description><c>X_UI_Controller_HUD</c></description></item>
 * <item><description><c>X_Character_NPC</c></description></item>
 * </list>
 * </remarks>
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FX_UIAttributeDisplayCurrentMaxPayloadSignature, const FX_UIAttributeDisplayCurrentMaxPayload&, DisplayInfo);