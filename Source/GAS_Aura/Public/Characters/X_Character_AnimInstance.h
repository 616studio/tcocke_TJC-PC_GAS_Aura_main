// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "X_Character_AnimInstance.generated.h"

class UCharacterMovementComponent;
 /**
 * Caches references common to Characters used in their ABPs.
 */
UCLASS()
class GAS_AURA_API UX_Character_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
#pragma region Unreal Defaults
	
public:
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	
private:
	
#pragma endregion Unreal Defaults
	
#pragma region Custom
	
public:
	
protected:
	
	/** Cached reference to owning Character. */
	UPROPERTY(BlueprintReadOnly, Category = "***CUSTOM|Animation")
	TWeakObjectPtr<ACharacter> CharacterOwner;
	
	/** Cached reference to owning Character's CharacterMovementComponent. */
	UPROPERTY(BlueprintReadOnly, Category = "***CUSTOM|Animation")
	TWeakObjectPtr<UCharacterMovementComponent> CharMoveComp;
	
	/** Cached 2D (XY) vector length for velocity used for blend spaces. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|Animation")
	float GroundSpeed = 0.0f;
	
	/** Cached acceleration state. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|Animation")
	bool bIsAccelerating = false;
			
private:
	
	/** Helper function. */
	void CacheReferences();
	
#pragma endregion Custom
};
