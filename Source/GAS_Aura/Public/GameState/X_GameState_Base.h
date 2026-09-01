// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "X_GameState_Base.generated.h"

// Forward declarations
class UX_CharacterClassInfo;

/**
 * <summary>
 * The custom base class for the Game State.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Holds globally accessible data that must be replicated or accessed by both the Server and Clients (e.g., combat calculation data for local prediction).</description></item>
 * </list>
 * </remarks>
 */
UCLASS()
class GAS_AURA_API AX_GameState_Base : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	
	/**
	 * <summary>
	 * Data Asset <c>DA_CharacterClassInfo</c> referenced here so it is accessible to Clients for local GAS prediction.
	 * </summary> 
	 */
	UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Character Class Defaults")
	TObjectPtr<UX_CharacterClassInfo> CharacterClassInfo;
};
