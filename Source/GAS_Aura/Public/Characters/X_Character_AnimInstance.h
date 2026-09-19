// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "X_Character_AnimInstance.generated.h"

class UCharacterMovementComponent;
 /**
 * <summary>
 * Caches references common to Characters used in their ABPs.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>When creating an ABP for a Character, make sure to use specify class as the Parent Class.</description></item>
 * </list>
 * </remarks>
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
	
	/** 
	 * <summary>
	 * Cached reference to owning Character.
	 * </summary>
	 */
	UPROPERTY(BlueprintReadOnly, Category = "***CUSTOM|Animation")
	TWeakObjectPtr<ACharacter> CharacterOwner;
	
	/** 
	 * <summary>
	 * Cached reference to owning Character's CharacterMovementComponent. 
	 * </summary>
	 */
	UPROPERTY(BlueprintReadOnly, Category = "***CUSTOM|Animation")
	TWeakObjectPtr<UCharacterMovementComponent> CharMoveComp;
	
	/** 
	 * <summary>
	 * Cached 2D (XY) vector length for velocity used for blend spaces. 
	 * </summary>
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|Animation")
	float GroundSpeed = 0.0f;
	
	/** 
	 * <summary>
	 * Cached acceleration state. 
	 * </summary>
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|Animation")
	bool bIsAccelerating = false;
			
private:
	
	/** 
	 * <summary>
	 * Helper function to cache references to <c>CharacterOwner</c> and <c>CharMoveComp</c>. 
	 * </summary>
	 */
	void CacheReferences();
	
#pragma endregion Custom
};
