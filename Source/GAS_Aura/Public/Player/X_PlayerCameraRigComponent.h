// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "X_PlayerCameraRigComponent.generated.h"

class UCameraComponent;
class USpringArmComponent;

/**
 * <summary>
 * Actor Component responsible for managing the player Character's camera perspective and movement constraints.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Configures the isometric, top-down ARPG perspective and restricts movement to a flat 2D plane for point-and-click navigation.</description></item>
 * </list>
 * </remarks>
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_AURA_API UX_PlayerCameraRigComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Unreal Defaults
	
public:
	
	UX_PlayerCameraRigComponent();
	
	virtual void BeginPlay() override;
	
protected:
	

private:
	
#pragma endregion Unreal Defaults

#pragma region Custom

public:
	
	/**
	 * <summary>
	 * Connects this component to the player Character's physical camera elements.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Call this from the constructor in <c>X_Character_Player</c> to set everything up after creating the subobjects.</description></item>
	 * <item><description>Detaches the camera from the pawn's rotation so the isometric view doesn't spin when the character turns around.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="InBoom">[<c>USpringArmComponent*</c>]: The Spring Arm attached to the player's Character.</param>
	 * <param name="InCamera">[<c>UCameraComponent*</c>]: The Camera attached to the Spring Arm.</param>
	 */
	void InitCamera(USpringArmComponent* InBoom, UCameraComponent* InCamera);
	
	/**
 * <summary>
 * Attempts to sync the PlayerController's ControlRotation to the CameraBoom's rotation.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Ensures the top-down camera's fixed isometric angle dictates the concept of "Forward" for the player's inputs. Without this sync, pressing 'W' might move the character along world-North instead of screen-Up.</description></item>
 * <item><description>Called automatically in <c>BeginPlay</c> (via <c>ApplyCameraSettings</c>).</description></item>
 * <item><description>If the Controller isn't set yet (ex: dynamically spawned Characters), the owning Character should call this again from <c>PossessedBy</c> as a fallback.</description></item>
 * </list>
 * </remarks>
 */
	void TrySyncControlRotation();

protected:

private:
	
	UPROPERTY()
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY()
	TObjectPtr<UCameraComponent> PlayerCamera;

	/**
	 * <summary>
	 * Configures the player Character's rotation and movement settings.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Applies top-down ARPG behavior: disabling controller yaw, enabling orient-to-movement for point-and-click turning, and locking the character to the XY plane.</description></item>
	 * </list>
	 * </remarks>
	 */
	void ApplyCameraSettings();

	/**
	 * <summary>
	 * Tracks whether the PlayerController's ControlRotation has been successfully synced to the CameraBoom.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Prevents redundant calls if both <c>BeginPlay</c> and <c>PossessedBy</c> succeed.</description></item>
	 * </list>
	 * </remarks>
	 */
	bool bControlRotationSynced = false;
	
#pragma endregion Custom
	
};
