// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))


#include "AbilitySystem/X_AbilitySystemLibrary.h"

#include "GameplayEffectTypes.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "Player/X_PlayerState.h"
#include "UI/HUD/X_HUD.h"
#include "UI/Controllers/X_UI_Controller_Base.h"

class AX_PlayerState;

#pragma region UI

UX_UI_Controller_AttributeMenu* UX_AbilitySystemLibrary::GetView_AttributeMenuController(UObject* WorldContextObject)
{
	AX_HUD* HUD = nullptr;
	FModelsPayload ModelsPayload;
	
	if (TryGetModels(WorldContextObject, HUD, ModelsPayload))
	{
		return HUD->GetView_AttributeMenuController(ModelsPayload);
	}

	return nullptr;
}

UX_UI_Controller_HUD* UX_AbilitySystemLibrary::GetView_HUDController(UObject* WorldContextObject)
{
	AX_HUD* HUD = nullptr;
	FModelsPayload ModelsPayload;

	if (TryGetModels(WorldContextObject, HUD, ModelsPayload))
	{
		return HUD->GetView_HUDController(ModelsPayload);
	}

	return nullptr;
}



bool UX_AbilitySystemLibrary::TryGetModels(UObject* WorldContextObject, AX_HUD*& OutHUD, FModelsPayload& OutPayload)
{
	APlayerController* PC = GetPlayerControllerFromWorldContextObject(WorldContextObject);
	if (!PC) return false;

	OutHUD = Cast<AX_HUD>(PC->GetHUD());
	if (!OutHUD) return false;

	AX_PlayerState* PS = PC->GetPlayerState<AX_PlayerState>();
	if (!PS) return false;

	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	UAttributeSet* AS = PS->GetAttributeSet();

	if (!ASC || !AS) return false;

	OutPayload = FModelsPayload(PC, PS, ASC, AS);
	return true;
}

/*
bool UX_AbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FGameplayEffectContext* BaseContext = EffectContextHandle.Get())
	{
		// The safety check travels with the cast!
		if (BaseContext->GetScriptStruct() == FX_GameplayEffectContext::StaticStruct())
		{
			const FX_GameplayEffectContext* XContext = static_cast<const FX_GameplayEffectContext*>(BaseContext);
			return XContext->IsBlockedHit();
		}
		
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to FX_GameplayEffectContext! Ensure AbilitySystemGlobals is configured correctly.  UI combat messages for Blocked will not appear.  Function: %hs"), __FUNCTION__);
	}
	
	return false;
}

bool UX_AbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FGameplayEffectContext* BaseContext = EffectContextHandle.Get())
	{
		if (BaseContext->GetScriptStruct() == FX_GameplayEffectContext::StaticStruct())
		{
			const FX_GameplayEffectContext* XContext = static_cast<const FX_GameplayEffectContext*>(BaseContext);
			return XContext->IsCriticalHit();
		}

		UE_LOG(LogTemp, Error, TEXT("Failed to cast to FX_GameplayEffectContext! Ensure AbilitySystemGlobals is configured correctly.  UI combat messages for Blocked will not appear.  Function: %hs"), __FUNCTION__);
	}
	
	return false;
}

void UX_AbilitySystemLibrary::SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FGameplayEffectContext* BaseContext = EffectContextHandle.Get())
	{
		if (BaseContext->GetScriptStruct() == FX_GameplayEffectContext::StaticStruct())
		{
			FX_GameplayEffectContext* XContext = static_cast<FX_GameplayEffectContext*>(BaseContext);
			XContext->SetIsBlockedHit(bInIsBlockedHit);
			return;
		}
        
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to FX_GameplayEffectContext! Ensure AbilitySystemGlobals is configured correctly. Blocked hit flag was NOT set. Function: %hs"), __FUNCTION__);
	}
}

void UX_AbilitySystemLibrary::SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FGameplayEffectContext* BaseContext = EffectContextHandle.Get())
	{
		if (BaseContext->GetScriptStruct() == FX_GameplayEffectContext::StaticStruct())
		{
			FX_GameplayEffectContext* XContext = static_cast<FX_GameplayEffectContext*>(BaseContext);
			XContext->SetIsCriticalHit(bInIsCriticalHit);
			return;
		}
        
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to FX_GameplayEffectContext! Ensure AbilitySystemGlobals is configured correctly. Critical hit flag was NOT set. Function: %hs"), __FUNCTION__);
	}
}
*/

#pragma endregion UI

#pragma region Utility

APlayerController* UX_AbilitySystemLibrary::GetPlayerControllerFromWorldContextObject(UObject* WorldContextObject)
{
	if (APlayerController* PC = Cast<APlayerController>(WorldContextObject))
	{
		return PC;
	}

	if (const APawn* Pawn = Cast<APawn>(WorldContextObject))
	{
		return Cast<APlayerController>(Pawn->GetController());
	}

	if (const APlayerState* PS = Cast<APlayerState>(WorldContextObject))
	{
		return PS->GetPlayerController();
	}

	if (const UUserWidget* Widget = Cast<UUserWidget>(WorldContextObject))
	{
		return Widget->GetOwningPlayer();
	}

	// Fallback: return nullptr instead of "Player 0".
	// Returning "Player 0" would return the WRONG player's controller in multiplayer, resulting in the wrong HUD, ASC, and Attributes being used. 
	return nullptr;
}

/*
void UX_AbilitySystemLibrary::GetLivingCharactersWithinSphereRadius(UObject* WorldContextObject,
	TArray<AActor*>& OverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(
			Overlaps, 
			SphereOrigin, 
			FQuat::Identity, 
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), 
			FCollisionShape::MakeSphere(Radius), 
			SphereParams
		);
		
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UX_CharacterInterface>() && !IX_CharacterInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OverlappingActors.AddUnique(IX_CharacterInterface::Execute_GetAvatarActor(Overlap.GetActor()));
			}
		}
	}
}
*/

/*
FGameplayTag UX_AbilitySystemLibrary::GetResistanceTagForDamageType(UObject* WorldContextObject, const FGameplayTag& DamageTypeTag)
{
	if (const UX_CharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		// Search the map for the matching Damage Type and return the associated Resistance Attribute Gameplay Tag.
		if (const FGameplayTag* ResistanceTag = ClassInfo->DamageTypesToResistances.Find(DamageTypeTag))
		{
			return *ResistanceTag;
		}
	}
    
	// If an associated Resistance Attribute Gameplay Tag wasn't found (or the Data Asset is missing), return an empty Gameplay Tag.
	return FGameplayTag::EmptyTag;
}
*/

/*
UX_CharacterClassInfo* UX_AbilitySystemLibrary::GetCharacterClassInfo(UObject* WorldContextObject)
{
	// Route through GameState so Clients can access the data for local prediction.
	AX_GameState_Base* GameState = Cast<AX_GameState_Base>(UGameplayStatics::GetGameState(WorldContextObject));
	if (!GameState) return nullptr;
	
	UX_CharacterClassInfo* ClassInfo = GameState->CharacterClassInfo;
	if (!ensureMsgf(IsValid(ClassInfo), TEXT("Actor: %s - Missing data for Editor assigned variable (%s).  Function: %hs"),
				   *GameState->GetName(),
				   *GET_MEMBER_NAME_CHECKED(AX_GameState_Base, CharacterClassInfo).ToString(),
				   __FUNCTION__))
	{
		return nullptr;
	}
	
	return ClassInfo;
}
*/

#pragma endregion Utility