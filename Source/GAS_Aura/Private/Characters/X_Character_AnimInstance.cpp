// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))


#include "Characters/X_Character_AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#pragma region Unreal Defaults

void UX_Character_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	CacheReferences();
}

void UX_Character_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!CharacterOwner.IsValid() || !CharMoveComp.IsValid())
	{
		CacheReferences();
	}
	
	if (CharMoveComp.IsValid())
	{
		GroundSpeed = CharMoveComp->Velocity.Size2D();
		bIsAccelerating = CharMoveComp->GetCurrentAcceleration().SizeSquared() > 0.0f;
	}
}

#pragma endregion Unreal Defaults

#pragma region Custom

void UX_Character_AnimInstance::CacheReferences()
{
	if (ACharacter* Character = Cast<ACharacter>(TryGetPawnOwner()))
	{
		CharacterOwner = Character;
		CharMoveComp = Character->GetCharacterMovement();
	}
}
#pragma endregion Custom