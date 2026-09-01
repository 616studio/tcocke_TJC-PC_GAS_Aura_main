// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))


#include "Player/X_PlayerCameraRigComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#pragma region Unreal Defaults

UX_PlayerCameraRigComponent::UX_PlayerCameraRigComponent()
{
	// There's no reason for this class to Tick.
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UX_PlayerCameraRigComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ApplyCameraSettings();
}

#pragma endregion Unreal Defaults

#pragma region Custom

void UX_PlayerCameraRigComponent::InitCamera(USpringArmComponent* InBoom, UCameraComponent* InCamera)
{
    CameraBoom = InBoom;
    PlayerCamera = InCamera;

    // Create a fixed perspective where the camera stays at a consistent angle (specified in the Blueprint Editor) regardless of the direction the Character is facing.
    
    if (CameraBoom)
    {
        CameraBoom->bUsePawnControlRotation = false;
        CameraBoom->bInheritPitch = false;
        CameraBoom->bInheritRoll = false;
        CameraBoom->bInheritYaw = false;

        // Aesthetic behavior addition to the camera.
        CameraBoom->bEnableCameraLag = true;
    }

    if (PlayerCamera)
    {
        PlayerCamera->bUsePawnControlRotation = false;
    }
}

void UX_PlayerCameraRigComponent::ApplyCameraSettings()
{
    if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
    {
        // Prevent the Pawn from turning its body in the direction the Player is looking or aiming.
        OwnerCharacter->bUseControllerRotationYaw = false;
        OwnerCharacter->bUseControllerRotationPitch = false;
        OwnerCharacter->bUseControllerRotationRoll = false;

        if (UCharacterMovementComponent* MoveComp = OwnerCharacter->GetCharacterMovement())
        {
            // Rotate the character to face the direction they are accelerating.
            MoveComp->bOrientRotationToMovement = true;
            MoveComp->RotationRate = FRotator(0.0f, 400.0f, 0.0f);

            // Since this is a top-down style RPG, we want the Player locked to the XY plane.
            MoveComp->bConstrainToPlane = true;
            MoveComp->bSnapToPlaneAtStart = true;
            
        }      
    }

    // Attempt to sync the control rotation now.
    // For placed-in-level actors, the Controller is typically valid by BeginPlay.
    // For dynamically spawned actors, it may not be — the owning Character can call TrySyncControlRotation() from PossessedBy as a fallback.
    TrySyncControlRotation();
}

void UX_PlayerCameraRigComponent::TrySyncControlRotation()
{
    // Only sync once. Prevents redundant calls if both BeginPlay and PossessedBy succeed.
    if (bControlRotationSynced) return;

    // Force the PlayerController to match the fixed camera angle (rotation) so Character movement direction on screen matches Player input direction.
    if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
    {
        if (APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController()))
        {
            if (CameraBoom)
            {
                PC->SetControlRotation(CameraBoom->GetComponentRotation());
                bControlRotationSynced = true;
            }
        }
    }
}

#pragma endregion Custom

