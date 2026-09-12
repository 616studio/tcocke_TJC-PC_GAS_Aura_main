// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))


#include "Characters/X_Character_Player.h"
#include "AbilitySystem/X_AbilitySystemComponent.h"
#include "AbilitySystem/X_AttributeSet.h"
#include "Camera/CameraComponent.h"
#include "DataAssets/X_CharacterClassInfo.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameState/X_GameState_Base.h"
#include "Kismet/GameplayStatics.h"
#include "Player/X_PlayerCameraRigComponent.h"
#include "Player/X_PlayerController.h"
#include "Player/X_PlayerState.h"
#include "UI/HUD/X_HUD.h"

#pragma region Unreal Defaults

AX_Character_Player::AX_Character_Player()
{
	// No reason for this Character to Tick.
	PrimaryActorTick.bCanEverTick = false;
	
	InitCameraRig();
}

void AX_Character_Player::BeginPlay()
{
	Super::BeginPlay();
		
}

#pragma endregion Unreal Defaults

#pragma region Ability System

ECharacterClass AX_Character_Player::GetCharacterClass() const
{
	const AX_PlayerState* PS = GetPlayerState<AX_PlayerState>();

	if (!ensureMsgf(IsValid(PS), TEXT("Actor: %s - No valid (PS) found.  Function: %hs"),
	               *GetName(), __FUNCTION__))
	{
		return ECharacterClass::Unassigned;
	}
	
	return PS->GetPlayerCharacterClass();
}

int32 AX_Character_Player::GetCharacterLevel() const
{
	const AX_PlayerState* PS = GetPlayerState<AX_PlayerState>();
	
	if (!ensureMsgf(IsValid(PS), TEXT("Actor: %s - No valid (PS) found.  Function: %hs"),
				   *GetName(), __FUNCTION__))
	{
		return 1;
	}
	
	return PS->GetPlayerCharacterLevel();
}

void AX_Character_Player::SetCharacterLevel(int32 Level)
{
	AX_PlayerState* PS = GetPlayerState<AX_PlayerState>();
	
	if (!ensureMsgf(IsValid(PS), TEXT("Actor: %s - No valid (PS) found.  Function: %hs"),
				   *GetName(), __FUNCTION__))
	{
		return;
	}
	
	PS->SetPlayerCharacterLevel(Level);
}

void AX_Character_Player::SetCharacterClass(const ECharacterClass NewClassType)
{
	AX_PlayerState* PS = GetPlayerState<AX_PlayerState>();
	
	if (!ensureMsgf(IsValid(PS), TEXT("Actor: %s - No valid (PS) found.  Function: %hs"),
				   *GetName(), __FUNCTION__))
	{
		return;
	}
	
	PS->SetPlayerCharacterClass(NewClassType);
}

UAbilitySystemComponent* AX_Character_Player::GetAbilitySystemComponent() const
{
	const AX_PlayerState* PS = GetPlayerState<AX_PlayerState>();
	return PS ? PS->GetAbilitySystemComponent() : nullptr;
}

UAttributeSet* AX_Character_Player::GetAttributeSet() const
{
	const AX_PlayerState* PS = GetPlayerState<AX_PlayerState>();
	if (!ensureMsgf(IsValid(PS), TEXT("Actor: %s - No valid (PS) found.  Function: %hs"),
				   *GetName(), __FUNCTION__))
	{
		return nullptr;
	}
	
	return PS->GetAttributeSet();
}

void AX_Character_Player::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// Purely a sanity check despite PossessedBy being a server-only function.
	if (!HasAuthority()) return;
	
	// AUTHORITATIVE SERVER INIT (Dedicated & Listen Server)
	InitAbilitySystemServerSide();
	
	// LISTEN SERVER - HOST SAFEGUARD:
	// RepNotifies (OnRep_PlayerState / OnRep_Controller) DO NOT fire on the server for locally controlled pawns.
	// We must explicitly trigger client-side initialization if this authoritative pawn is locally controlled.
	if (IsLocallyControlled())
	{
		InitAbilitySystemClientSide();
	}
	
	// Fallback for the case where BeginPlay inside the CameraRigComp class (CameraRigComp->ApplyCameraSettings) fires before the Controller is set.
	// Guard built into TrySyncControlRotation prevents this from running if BeginPlay (CameraRigComp->ApplyCameraSettings) already succeeded. 
	if (CameraRigComp)
	{
		CameraRigComp->TrySyncControlRotation();
	}
}

void AX_Character_Player::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Client-side initialization (fires the exact frame PlayerState replicates)
	InitAbilitySystemClientSide();
	
}

void AX_Character_Player::OnRep_Controller()
{
	Super::OnRep_Controller();
	
	// Fallback client-side initialization to handle async Controller arrival.
	InitAbilitySystemClientSide();
	
	// Ensures the Client's control rotation is synced to the isometric camera angle.
	// Only attempt to sync the camera if we actually have a Controller.
	if (GetController() && CameraRigComp)
	{
		CameraRigComp->TrySyncControlRotation();
	}
}

void AX_Character_Player::InitAbilitySystemServerSide()
{
	// Fail-Safe: If PlayerState hasn't replicated/attached yet during custom spawning, log a non-fatal ensure to catch the execution order in PIE.
	AX_PlayerState* PS = GetPlayerState<AX_PlayerState>();
	if (!ensureMsgf(IsValid(PS), TEXT("%s: PossessedBy executed before PlayerState was valid!"), *GetName()))
	{
		return;
	}

	// We need our custom X_AbilitySystemComponent in order to call our custom function BindToGameplayEffectDelegate.
	UX_AbilitySystemComponent* XASC = Cast<UX_AbilitySystemComponent>(PS->GetAbilitySystemComponent());
	if (!IsValid(XASC)) 
	{
		UE_LOG(LogTemp, Error, TEXT("%s: ASC is missing or not of type UX_AbilitySystemComponent!"), *GetName());
		return; 
	}
	
	// We need our custom X_AttributeSet in order to initialize the Player's Attributes.
	UX_AttributeSet* XAS = Cast<UX_AttributeSet>(PS->GetAttributeSet());
	if (!IsValid(XAS)) 
	{
		UE_LOG(LogTemp, Error, TEXT("%s: AttributeSet is missing or not of type UX_AttributeSet! Aborting Initialization."), *GetName());
		return;
	}

	// This communicates to the ASC who its OwnerActor and AvatarActor are.
	// Will only work if called AFTER a Controller has been set (the PlayerController for the Player in this case).
	// This is why we call it inside PossessedBy because we know the Controller will be set on the SERVER by the time PossessedBy is called.
	XASC->InitAbilityActorInfo(PS, this);
	
	// Bind to the custom delegate.
	XASC->BindToGameplayEffectDelegate();
	
	// FIRST SPAWN: The Player State has no data yet. Grant everything.
	// Server-Side Guard: Grant initial attributes EXACTLY ONCE per PlayerState lifetime.
	if (!PS->HasGrantedStartupData())
	{
		InitializeAttributes(this, this, this, GetCharacterClass(), GetCharacterLevel());
		InitializeDefaultGameplayTags(this, this, this, GetCharacterClass(), GetCharacterLevel());
		GrantClassDefaultGameplayAbilitiesOnStartup(GetCharacterClass(), GetCharacterLevel());
		GrantClassSharedGameplayAbilitiesOnStartup(GetCharacterLevel());
		
		PS->SetHasGrantedStartupData(true);
	}

	// RESPAWN: The Player State already has abilities, tags, and secondary stat layers.
	// We ONLY need to re-apply the Vital Attributes to refill Health and Mana back to their default Max values.
	else
	{
		AX_GameState_Base* GameState = Cast<AX_GameState_Base>(UGameplayStatics::GetGameState(this));
		if (!ensureMsgf(IsValid(GameState), TEXT("Actor: %s - No valid (GameState) found.  Function: %hs"),
		               *GetName(), __FUNCTION__))
		{
			return;
		}
		
		UX_CharacterClassInfo* ClassInfo = GameState->CharacterClassInfo;
		if (ensureMsgf(IsValid(ClassInfo), TEXT("Actor: %s - No valid (ClassInfo) found.  Function: %hs"),
		               *GetName(), __FUNCTION__))
		{
			return;
		}
		
		const FX_CharacterClassDefaultInfo* ClassDefaultInfo = ClassInfo->GetCharacterClassDefaultInfo(GetCharacterClass());
		if (!ensureMsgf(ClassDefaultInfo, TEXT("Actor: %s - Missing CharacterClassInfo entry for CharacterClass: %s. Function: %hs"),
					*GetName(),
					*UEnum::GetValueAsString(GetCharacterClass()),
					__FUNCTION__))
		{
			return;
		}
		
		const TSubclassOf<UGameplayEffect> VitalAttributes = ClassDefaultInfo->VitalAttributes;
		if (!ensureMsgf(VitalAttributes, TEXT("Actor: %s - Missing data for Editor assigned variable (VitalAttributes) for CharacterClass:  %s.  Function: %hs"),
					   *GetName(),
					   *UEnum::GetValueAsString(GetCharacterClass()),
					   __FUNCTION__))
		{
			return;	
		}
		
		ApplyGameplayEffectToSelf(GetAbilitySystemComponent(), VitalAttributes, this, this, this, GetCharacterLevel());
	}
}

void AX_Character_Player::InitAbilitySystemClientSide()
{
	// Don't use asserts here due to asynchronous replication on the client side for OnRep_PlayerState.
	AX_PlayerState* PS = GetPlayerState<AX_PlayerState>();
	if (!IsValid(PS)) return;

	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	if (!ensureMsgf(IsValid(ASC), TEXT("Actor: %s - No valid (ASC) found.  Function: %hs"),
	               *GetName(), __FUNCTION__))
	{
		return;
	}

	// We need our custom X_AbilitySystemComponent in order to call our custom function BindToGameplayEffectDelegate.
	UX_AbilitySystemComponent* XASC = Cast<UX_AbilitySystemComponent>(ASC);
	if (!ensureMsgf(IsValid(XASC), TEXT("Actor: %s - No valid (XASC) found.  Function: %hs"),
	               *GetName(), __FUNCTION__))
	{
		return;
	}

	// We need our custom Attribute Set to pass into InitView_HUD.
	UX_AttributeSet* XAS = Cast<UX_AttributeSet>(PS->GetAttributeSet());
	if (!IsValid(XAS)) 
	{
		UE_LOG(LogTemp, Error, TEXT("%s: AttributeSet is missing or not of type UX_AttributeSet! Aborting Initialization."), *GetName());
		return;
	}

	// Guard ASC initialization so it only runs once.
	// Both OnRep_PlayerState and OnRep_Controller call this function, but we only need to initialize the ASC once.
	// We keep this separate from the HUD guard because the Controller might arrive on a different OnRep call than the PlayerState.
	if (!bAbilitySystemInitialized)
	{
		// This communicates to the ASC who its OwnerActor and AvatarActor are.
		ASC->InitAbilityActorInfo(PS, this);

		// Bind to our custom delegate.
		XASC->BindToGameplayEffectDelegate();

		bAbilitySystemInitialized = true;
	}

	// Simulated proxies don't need a HUD.
	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		// Mark as true so the function can successfully return.
		bHUDInitialized = true; 
		return;
	}
	
	TryInitHUD(PS, XASC, XAS);
}

void AX_Character_Player::TryInitHUD(AX_PlayerState* PS, UX_AbilitySystemComponent* XASC, UX_AttributeSet* XAS)
{
	if (bHUDInitialized || bHUDInitPending) return;

	AX_PlayerController* PC = Cast<AX_PlayerController>(GetController());
	if (!ensureMsgf(IsValid(PC), TEXT("Actor: %s - No valid (PC) found.  Function: %hs"),
	               *GetName(), __FUNCTION__))
	{
		return;
	}

	// Path A: Synchronous Init — PC and HUD are already valid when ASC finishes initializing.
	if (AX_HUD* HUD = Cast<AX_HUD>(PC->GetHUD()))
	{
		HUD->InitHUD(PC, PS, XASC, XAS);
		bHUDInitialized = true;
		return;
	}

	// Path B: Asynchronous Fallback — ASC is ready, but HUD actor is still replicating via ClientSetHUD RPC.
	bHUDInitPending = true;
	
	// AddWeakLambda guards 'this'. 
	// Re-fetch models dynamically inside the lambda body to guarantee valid GC-tracked pointers.
	PC->OnHUDInitializedDelegate.AddWeakLambda(this, [this](AX_HUD* ReadyHUD)
	{
		AX_PlayerController* CurrentPC = Cast<AX_PlayerController>(GetController());
		AX_PlayerState* CurrentPS = GetPlayerState<AX_PlayerState>();

		if (!CurrentPC || !CurrentPS) return;

		UX_AbilitySystemComponent* CurrentXASC = Cast<UX_AbilitySystemComponent>(CurrentPS->GetAbilitySystemComponent());
		UX_AttributeSet* CurrentXAS = Cast<UX_AttributeSet>(CurrentPS->GetAttributeSet());

		if (!bHUDInitialized && bAbilitySystemInitialized && IsValid(ReadyHUD) && IsValid(CurrentPC) && IsValid(CurrentPS) && IsValid(CurrentXASC) && IsValid(CurrentXAS))
		{
			ReadyHUD->InitHUD(CurrentPC, CurrentPS, CurrentXASC, CurrentXAS);
			bHUDInitialized = true;
			bHUDInitPending = false;
		}
	});
}

#pragma endregion Ability System

#pragma region Camera Rig

void AX_Character_Player::InitCameraRig()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCamera->SetupAttachment(CameraBoom);

	CameraRigComp = CreateDefaultSubobject<UX_PlayerCameraRigComponent>("CameraRigComp");

	CameraRigComp->InitCamera(CameraBoom, PlayerCamera);
}

void AX_Character_Player::OnRep_ReplicatedMovement()
{
	// Is this the Character currently controlled by the local player?  We ONLY want to apply this fix for the person playing.
	// Everyone else (Simulated Proxies) should just snap to whatever the server says because we don't predict their input.
	if (IsLocallyControlled())
	{
		// We want to trust our local rotation logic (prediction) because it is instant and feels responsive.
		// The server is always slightly behind us due to latency.        
		// Capture our current smooth local rotation before the server overwrites it.
		const FRotator LocalRotation = GetActorRotation();

		// Call the Super implementation.
		// This applies the Server's authoritative Position and Rotation.
		Super::OnRep_ReplicatedMovement();

		// Re-apply our local rotation.
		// Result: We accept the Server's Position correction (preventing de-sync), but we keep our smooth Rotation.
		SetActorRotation(LocalRotation);
	}
	else
	{
		// If this is another player (Simulated Proxy), we want to see exactly what the server tells us they are doing and apply it immediately.
		Super::OnRep_ReplicatedMovement();
	}
}

#pragma endregion Camera Rig