// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#include "Player/X_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "Components/SplineComponent.h"
#include "GAS_Aura/UtilityClasses/X_CollisionProfiles.h"
#include "Interfaces/X_CursorHighlightInterface.h"
#include "UI/HUD/X_HUD.h"
#include "UI/UserWidgets/X_FloatingDamageTextWidget.h"

#pragma region Unreal Defaults

AX_PlayerController::AX_PlayerController()
{
	bReplicates = true;
	SplinePath = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AX_PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	PerformAutoMove();
}

void AX_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitInput();
	InitMouseCursor();
	
	GetWorldTimerManager().SetTimer(
		CursorTraceTimerHandle,
		this,
		&ThisClass::PerformCursorTrace,
		CursorTraceFrequency,
		true
	);
		
}

void AX_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		if (ensureMsgf(IA_Move, TEXT("Actor: %s - Missing Editor assigned variable (%s).  Assign in Blueprint Editor: CUSTOM - Input.  Function: %hs"),
				*GetName(), *GET_MEMBER_NAME_CHECKED(ThisClass, IA_Move).ToString(), __FUNCTION__))
			{
				EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ThisClass::PerformMove);
			}
	}
}

#pragma endregion Unreal Defaults

#pragma region HUD Initialization

void AX_PlayerController::ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass)
{
	// Check for missing HUDClass in GameMode.
	if (!NewHUDClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: ClientSetHUD received a NULL HUDClass! Assign a valid AX_HUD class in your GameMode defaults."), *GetName());
		ensureMsgf(false, TEXT("%s: ClientSetHUD received a NULL HUDClass! Check GameMode settings."), *GetName());
        
		Super::ClientSetHUD_Implementation(NewHUDClass);
		return;
	}

	Super::ClientSetHUD_Implementation(NewHUDClass);

	// Check for class mismatch (HUD exists, but is not an AX_HUD subclass).
	if (AX_HUD* TargetHUD = Cast<AX_HUD>(GetHUD()))
	{
		OnHUDInitializedDelegate.Broadcast(TargetHUD);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Spawned HUD (%s) is not derived from AX_HUD!"), *GetName(), *GetNameSafe(GetHUD()));
		ensureMsgf(false, TEXT("%s: Spawned HUD is not an AX_HUD subclass!"), *GetName());
	}
}

#pragma endregion HUD Initialization

#pragma region Enhanced Input Subsystem

void AX_PlayerController::PerformMove(const struct FInputActionValue& Value)
{
	// Retrieve the 2D axis values from our IA and store them.
	const FVector2D InputAxisVector = Value.Get<FVector2D>();
		
	// Get the forward facing direction of our Controller.
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// Cancel any auto-moving if the Player activates any of the movement IAs.
		bIsAutoMoving = false;
		
		// Forward direction movement is dictated by the Y value in our 2D InputAxisVector (W (+1) and S (-1)). 
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		
		// Right direction movement is dictated by the X value in our 2D InputAxisVector (D (+1) and A (-1)).
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AX_PlayerController::InitInput() const
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (ensureMsgf(IMC_Player, TEXT("Actor: %s - Missing Editor assigned variable (%s).  Function: %hs"),
			*GetName(),
			*GET_MEMBER_NAME_CHECKED(ThisClass, IMC_Player).ToString(),
			__FUNCTION__))
		{
			// We only have 1 IMC for now, so priority can stay at 0.
			Subsystem->AddMappingContext(IMC_Player, 0);
		}
	}
}
	
#pragma endregion Enhanced Input System

#pragma region Highlightable Actor Behavior

void AX_PlayerController::PerformCursorTrace()
{
	FHitResult CursorHit;

	GetHitResultUnderCursor(ECC_Highlightable, false, CursorHit);

	if (!CursorHit.bBlockingHit)
	{
		// If there is no blocking hit AND Previous has an Actor assigned, it means the last frame WAS a blocking hit and Current's Actor is still highlighted.
		// At the end of the last frame, Current's Actor was copied to Previous.
		// Instead of calling RemoveHighlightFromActor on both Previous and Current, we can simply call it once on Previous since they both point to the same Actor in memory.
		if (PreviousHighlightedActor.IsValid())
		{
			PreviousHighlightedActor->RemoveHighlightFromActor();
		}
   
		// Now that the highlight has been removed from Current's Actor, we can clear out both pointers.
		CurrentHighlightedActor = nullptr;
		PreviousHighlightedActor = nullptr;

		return;
	}

	// If the cursor is hovering over our own Player Character, set Current to null so we ignore it.
	AActor* HitActor = CursorHit.GetActor();
	if (HitActor == GetPawn())
	{
		CurrentHighlightedActor = nullptr;
	}
	else
	{
		// Validate that the hit actor actually implements the CursorHighlightInterface.
		CurrentHighlightedActor = Cast<IX_CursorHighlightInterface>(HitActor);
	}
	
	// If the cursor is still over the same Actor from the previous frame, leave.
	if (CurrentHighlightedActor == PreviousHighlightedActor) return;

	// If the cursor is over a different Actor from the previous frame, highlight the new one...
	if (CurrentHighlightedActor.IsValid())
	{
		CurrentHighlightedActor->AddHighlightToActor();
	}
	
	// Remove the highlight from the previous one.
	if (PreviousHighlightedActor.IsValid())
	{
		PreviousHighlightedActor->RemoveHighlightFromActor();
	}

	// Store a reference to the Current highlighted Actor in Previous so we can compare them next frame.
	PreviousHighlightedActor = CurrentHighlightedActor;
}

#pragma endregion Highlightable Actor Behavior

#pragma region Top-Down RPG Mouse Click Behavior

void AX_PlayerController::InitMouseCursor()
{
	// Since this is a top-down ARPG, we want the default mouse cursor to be visible immediately on BeginPlay.
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	// Data structure used to set an input mode that allows the UI to respond to user input.
	// If the UI doesn't handle it, then the player input / PlayerController gets a chance.
	FInputModeGameAndUI InputModeData;

	// Do not lock mouse cursor to viewport.
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	// Do not hide the cursor when clicking.
	InputModeData.SetHideCursorDuringCapture(false);

	// Apply settings.
	SetInputMode(InputModeData);
}

void AX_PlayerController::PerformAutoMove()
{
	if (!bIsAutoMoving) return;
	
	// Check if Pawn is pending kill or garbage collection.
	APawn* ControlledPawn = GetPawn();
	if (!IsValid(ControlledPawn)) return;
	
	// Get the next closest location on the spline.
	const FVector LocationOnSpline = SplinePath->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
	
	/*
	// Get the direction to the next closest location on the spline.
	const FVector Direction = SplinePath->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);

	// Keep the Character moving in the direction to the next closest location on the spline.
	ControlledPawn->AddMovementInput(Direction);

	// Constantly calculate the distance to the mouse clicked destination.
	const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
	*/
	
	// Find our exact position along the spline as an "input key".
	const float CurrentInputKey = SplinePath->FindInputKeyClosestToWorldLocation(ControlledPawn->GetActorLocation());
	
	// Convert that "input key" to physical distance along the spline.
	const float CurrentDistance = SplinePath->GetDistanceAlongSplineAtSplineInputKey(CurrentInputKey);
	
	// Put the next point on the spline 100 units ahead of our current distance.
	const float LookAheadDistance = CurrentDistance + 100.0f;
	
	// Get the 3D world location of that next point.
	const FVector LookAheadLocation = SplinePath->GetLocationAtDistanceAlongSpline(LookAheadDistance, ESplineCoordinateSpace::World);
	
	// Move TOWARDS the next point, forcing the Character to pull back to the center of the path if they drift.
	const FVector Direction = (LookAheadLocation - ControlledPawn->GetActorLocation()).GetSafeNormal();
	
	ControlledPawn->AddMovementInput(Direction);

	// End auto-moving if we reach our destination within our acceptance radius.
	const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
	if (DistanceToDestination <= AutoRunAcceptanceRadius)
	{
		bIsAutoMoving = false;
	}
	
	/*
	// End auto-moving if we reach our destination within our acceptance radius.
	if (DistanceToDestination <= AutoRunAcceptanceRadius)
	{
		bIsAutoMoving = false;
	}
	*/
}

#pragma endregion Top-Down RPG Mouse Click Behavior

#pragma region Floating Damage Text

void AX_PlayerController::ShowFloatingDamageText_Local(const float DamageAmount, const FVector TargetLocation, const AActor* InstigatorActor, const FGameplayTag MessageTag, const FGameplayTag AbilityTag)
{
	// Ensure the widget class was assigned in the Editor.
	if (!ensureMsgf(FloatingDamageTextWidgetClass, TEXT("Actor: %s - Missing data for Editor assigned variable (%s).  Function: %hs"),
				   *GetName(),
				   *GET_MEMBER_NAME_CHECKED(ThisClass, FloatingDamageTextWidgetClass).ToString(),
				   __FUNCTION__))
	{
		return;
	}
	
	UX_FloatingDamageTextWidget* FloatingDamageText = CreateWidget<UX_FloatingDamageTextWidget>(this, FloatingDamageTextWidgetClass);
	if (FloatingDamageText)
	{
		// Check if the local Player's Pawn is the one who caused the damage.
		const bool bIsMyDamage = (GetPawn() == InstigatorActor);
      
		// Pass the data to the Blueprint.
		FloatingDamageText->SetupFloatingDamageText(DamageAmount, bIsMyDamage, TargetLocation, MessageTag, AbilityTag);
		FloatingDamageText->AddToViewport();
	}
}

#pragma endregion Floating Damage Text