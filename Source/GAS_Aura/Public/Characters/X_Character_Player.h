// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "X_Character_Base.h"
#include "X_Character_Player.generated.h"

class UX_AttributeSet;
class UX_AbilitySystemComponent;
class AX_PlayerState;
class UX_PlayerCameraRigComponent;
class USpringArmComponent;
class UCameraComponent;
class UAbilitySystemComponent;
class UAttributeSet;

/**
 * <summary>
 * The Player specific Character class.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Unlike NPCs, the Player Character does <b>NOT</b> own its own Ability System Component or Attribute Set.</description></item>
 * <item><description>It acts strictly as the "Avatar" (the physical representation in the world), while the <c>AX_PlayerState</c> acts as the "Owner" (the persistent data container).</description></item>
 * </list>
 * </remarks>
 */
UCLASS()
class GAS_AURA_API AX_Character_Player : public AX_Character_Base
{
	GENERATED_BODY()

#pragma region Unreal Defaults
	
public:
	
	AX_Character_Player();
	virtual void BeginPlay() override;
	
protected:
	
private:
	
#pragma endregion Unreal Defaults
	
#pragma region Ability System
	
public:
	
	virtual int32 GetCharacterLevel() const override;
	
	virtual void SetCharacterLevel(int32 Level) override;
	
	virtual ECharacterClass GetCharacterClass() const override;
	
	virtual void SetCharacterClass(const ECharacterClass NewClassType) override;
	
	/**
	 * <summary>
	 * <c>IAbilitySystemInterface</c> implementation (overriden from <c>X_Character_Base</c>). Returns the Ability System Component constructed and managed by <c>X_PlayerState</c>.
	 * </summary>
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	/**
	 * <summary>
	 * Returns the Attribute Set associated with this Character constructed and managed by <c>X_PlayerState</c> (overriden from <c>X_Character_Base</c>).
	 * </summary>
	 */
	virtual UAttributeSet* GetAttributeSet() const override;
	
	/**
	* <summary>
	* Used for the server-side initialization logic required for GAS.
	* </summary>
	* <remarks>
	* <b>ARCHITECTURE NOTES:</b>
	* <list type="bullet">
	* <item><description>When <c>PossessedBy</c> runs, the PlayerState, Controller, and Pawn are all spawned, linked, and valid in memory immediately (synchronous execution).</description></item>
	* <item><description>Initializes the ASC with <c>X_PlayerState</c> as the <c>OwnerActor</c> and <c>this</c> Character as the <c>AvatarActor</c>.</description></item>
	* <item><description>Also used by the <c>CameraRigComp</c> as a fallback for the <c>TrySyncControlRotation</c> race condition.</description></item>
	* </list>
	* </remarks>
	* <param name="NewController">[<c>AController*</c>]: The controller possessing this Pawn.</param>
	*/
	virtual void PossessedBy(AController* NewController) override;
	
	/**
	 * <summary>
	 * Used for the client-side initialization logic required for GAS.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Called when the PlayerState has successfully replicated to the Client.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void OnRep_PlayerState() override;
	
	/**
	 * <summary>
	 * Fallback client-side possession and activation trigger for GAS.  
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Used to catch asynchronous replication race conditions where the PlayerState arrived first, but we were forced to wait on the Controller to arrive before initializing the HUD.</description></item>
	 * <item><description>Also used by the <c>CameraRigComp</c> to ensure the Client's control rotation is synced to the camera angle.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void OnRep_Controller() override;
	
protected:
	
private:

	/**
	 * <summary>
	 * Helper function for client-side initialization of the Ability System and HUD.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Async Replication Safety:</b> Handles non-deterministic client network replication timing between <c>OnRep_PlayerState</c> and <c>OnRep_Controller</c>.</description></item>
	 * </list>
	 * </remarks>
	 */
	void InitAbilitySystemClientSide();
	
	/**
	 * <summary>
	 * Helper function for server-side initialization of the Ability System and HUD.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Synchronous Execution:</b> Executes deterministically on the Server inside <c>PossessedBy</c>, where <c>PlayerState</c>, <c>Controller</c>, <c>ASC</c>, and <c>AttributeSet</c> are linked synchronously in authoritative memory on frame 0.</description></item>
	 * </list>
	 * </remarks>
	 */
	void InitAbilitySystemServerSide();

	/**
	 * <summary>
	 * Race condition guard for Ability System initialization.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Handles the race condition between <c>OnRep_PlayerState</c> and <c>OnRep_Controller</c> due to unpredictable asynchronous replication.</description></item>
	 * <item><description>Prevents double-initialization of the Ability System.</description></item>
	 * </list>
	 * </remarks>
	 */
	bool bAbilitySystemInitializedOnClient = false;
	
	/**
	 * <summary>
	 * Race condition guard for HUD initialization.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Handles the race condition between <c>OnRep_PlayerState</c> and <c>OnRep_Controller</c> due to unpredictable asynchronous replication.</description></item>
	 * <item><description>Prevents double-initialization of the HUD.</description></item>
	 * </list>
	 * </remarks>
	 */
	bool bHUDInitialized = false;
	
	/** 
	 * <summary>
	 * Guards against registering multiple delegate listeners while HUD replication is pending.
	 * </summary>
	 */ 
	bool bHUDInitPending = false;

	/**
	 * <summary>
	 * Helper function to consolidate the HUD initialization process.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Evaluates HUD readiness synchronously or registers a GC-safe weak event callback if pending.</description></item>
	 * </list>
	 * </remarks>
	 */
	void TryInitHUD(AX_PlayerState* PS, UX_AbilitySystemComponent* XASC, UX_AttributeSet* XAS);
		
#pragma endregion Ability System
	
#pragma region Camera
	
public:
	
	/**
	 * <summary>
	 * Overrides the native replication notification for movement to prevent rotation "jitter" in multiplayer.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>The Problem:</b> When <c>bOrientRotationToMovement</c> is true, conflicts between Local Prediction and the Server's Reality cause severe visual jitter on the client.</description></item>
	 * <item><description><b>The Solution:</b> This override intercepts the Server's location correction but rejects the Server's rotation correction, preserving the smooth local rotation.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void OnRep_ReplicatedMovement() override;
	
protected:

	/**
	 * <summary>
	 * The Spring Arm Component used to distance the camera behind the Player.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Pitched sharply downward to create an isometric, top-down view of the battlefield.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(VisibleAnywhere, Category = "***CUSTOM|Camera Components")
	TObjectPtr<USpringArmComponent> CameraBoom;

	/**
	 * <summary>
	 * The actual Camera Component used to view the world.
	 * </summary>
	 */
	UPROPERTY(VisibleAnywhere, Category = "***CUSTOM|Camera Components")
	TObjectPtr<UCameraComponent> PlayerCamera;

	/**
	 * <summary>
	 * Manages the camera behavior as an ActorComponent to keep its isolated logic out of this class.
	 * </summary>
	 */
	UPROPERTY(VisibleAnywhere, Category = "***CUSTOM|Camera Components")
	TObjectPtr<UX_PlayerCameraRigComponent> CameraRigComp;

private:

	/**
	 * <summary>
	 * Helper function to initialize the Camera components and rig.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>This function contains <c>CreateDefaultSubobject</c> and <b>MUST ONLY</b> be called from the Constructor!</description></item>
	 * </list>
	 * </remarks>
	 */
	void InitCameraRig();
	
#pragma endregion Camera
	
};
