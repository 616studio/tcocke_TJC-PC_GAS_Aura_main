// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))


#include "Characters/X_Character_Player.h"

#include "AbilitySystem/X_AbilitySystemComponent.h"
#include "AbilitySystem/X_AttributeSet.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
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

UAbilitySystemComponent* AX_Character_Player::GetAbilitySystemComponent() const
{
	const AX_PlayerState* PS = GetPlayerState<AX_PlayerState>();
	return PS ? PS->GetAbilitySystemComponent() : nullptr;
}

UAttributeSet* AX_Character_Player::GetAttributeSet() const
{
	const AX_PlayerState* PS = GetPlayerState<AX_PlayerState>();
	return PS ? PS->GetAttributeSet() : nullptr;
}

void AX_Character_Player::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// Purely a sanity check despite PossessedBy being a server-only function.
	if (!HasAuthority()) return;
	
	// DEDICATED SERVER
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
	
	// Ensure the Client's control rotation is synced to the isometric camera angle.
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
	if (!IsValid(PS)) 
	{
		UE_LOG(LogTemp, Error, TEXT("%s: PossessedBy executed before PlayerState was valid!"), *GetName());
		return; 
	}

	// The PlayerState contains the custom ASC and AS specific to the Player, so if we can't get them there's no point in continuing.
	// We need our custom X_AbilitySystemComponent in order to call our custom function BindToGameplayEffectDelegates, so if we can't get it there's no point in continuing.
	UX_AbilitySystemComponent* XASC = Cast<UX_AbilitySystemComponent>(PS->GetAbilitySystemComponent());
	if (!IsValid(XASC)) 
	{
		UE_LOG(LogTemp, Error, TEXT("%s: ASC is missing or not of type UX_AbilitySystemComponent!"), *GetName());
		return; 
	}

	// We need our custom Attribute Set to pass into InitView_HUD, so if we can't get it there's no point in continuing.
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
	
	// Bind to the ToTarget and ToSelf custom delegates.
	XASC->BindToGameplayEffectDelegate();
	
	// FIRST SPAWN: The Player State has no data yet. Grant everything.
	// Server-Side Guard: Grant initial attributes EXACTLY ONCE per PlayerState lifetime
	if (!PS->HasGrantedStartupData())
	{
		//InitializeAttributes(this, this, this, GetCharacterLevel());
		//InitializeDefaultGameplayTags(this, this, this, GetCharacterLevel());
		//GrantClassDefaultGameplayAbilitiesOnStartup(GetCharacterLevel());
		//GrantClassSharedGameplayAbilitiesOnStartup(GetCharacterLevel());
		
		// Temporary init to be replaced by Gameplay Effect initialization.
		XAS->InitHealth(50.0f);
		XAS->InitMaxHealth(100.0f);
		XAS->InitMana(50.0f);
		XAS->InitMaxMana(100.0f);

		PS->SetHasGrantedStartupData(true);
	}

	/*
	// RESPAWN: The Player State already has abilities, tags, and secondary stat layers.
	else
	{
		/*
		// We ONLY need to re-apply the Vital Attributes to refill Health and Mana back to their default Max values.
		AX_GameState_Base* GameState = Cast<AX_GameState_Base>(UGameplayStatics::GetGameState(this));
		if (GameState && GameState->CharacterClassInfo)
		{
			TSubclassOf<UGameplayEffect> VitalAttributes = GameState->CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass).VitalAttributes;
			if (VitalAttributes)
			{
				ApplyGameplayEffectToSelf(GetAbilitySystemComponent(), VitalAttributes, this, this, this, GetCharacterLevel());
			}
		}
	}
	*/
}

void AX_Character_Player::InitAbilitySystemClientSide()
{
	AX_PlayerState* PS = GetPlayerState<AX_PlayerState>();
	if (!IsValid(PS)) return;

	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	if (!IsValid(ASC)) return;

	// We need our custom X_AbilitySystemComponent here because BindToGameplayEffectDelegates only exists on our custom X_AbilitySystemComponent.
	UX_AbilitySystemComponent* XASC = Cast<UX_AbilitySystemComponent>(ASC);
	if (!IsValid(XASC)) return;

	// Strict Sanity Check: Ensure the Server has our specific custom AttributeSet.
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

		// Bind to delegates in our custom XASC.
		XASC->BindToGameplayEffectDelegate();

		bAbilitySystemInitialized = true;
	}

	// Simulated proxies don't need a HUD.
	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		// Mark as true so the function can successfully return and kill the timer.
		bHUDInitialized = true; 
		return;
	}
	
	TryInitHUD(PS, XASC, XAS);
}

void AX_Character_Player::TryInitHUD(AX_PlayerState* PS, UX_AbilitySystemComponent* XASC, UX_AttributeSet* XAS)
{
	if (bHUDInitialized || bHUDInitPending) return;

	AX_PlayerController* PC = Cast<AX_PlayerController>(GetController());
	if (!IsValid(PC)) return;

	// Path A: Synchronous Init — PC and HUD are already valid when ASC finishes initializing.
	if (AX_HUD* HUD = Cast<AX_HUD>(PC->GetHUD()))
	{
		HUD->InitView_HUD(PC, PS, XASC, XAS);
		bHUDInitialized = true;
		return;
	}

	// Path B: Asynchronous Fallback — ASC is ready, but HUD actor is still replicating via ClientSetHUD RPC.
	bHUDInitPending = true;
	
	// AddWeakLambda guards 'this'. Re-fetch models dynamically inside the lambda body to guarantee valid GC-tracked pointers.
	PC->OnHUDInitializedDelegate.AddWeakLambda(this, [this](AX_HUD* ReadyHUD)
	{
		AX_PlayerController* CurrentPC = Cast<AX_PlayerController>(GetController());
		AX_PlayerState* CurrentPS = GetPlayerState<AX_PlayerState>();

		if (!CurrentPC || !CurrentPS) return;

		UX_AbilitySystemComponent* CurrentXASC = Cast<UX_AbilitySystemComponent>(CurrentPS->GetAbilitySystemComponent());
		UX_AttributeSet* CurrentXAS = Cast<UX_AttributeSet>(CurrentPS->GetAttributeSet());

		if (!bHUDInitialized && bAbilitySystemInitialized && IsValid(ReadyHUD) && IsValid(CurrentPC) && IsValid(CurrentPS) && IsValid(CurrentXASC) && IsValid(CurrentXAS))
		{
			ReadyHUD->InitView_HUD(CurrentPC, CurrentPS, CurrentXASC, CurrentXAS);
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