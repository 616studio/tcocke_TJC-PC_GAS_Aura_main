// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "X_GameState_Base.generated.h"

// Forward declarations
class UX_CharacterClassInfo;

/**
 * <summary>
 * Holds globally accessible replicated data for both the Server and Clients.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Preserves server-authoritative initialization while allowing local clients to access the same data for prediction and UI queries.</description></item>
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
	 * Data Asset <c>DA_CharacterClassInfo</c> referenced here so it is accessible to both Servers and Clients.
	 * </summary> 
	 */
	UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Character Class Defaults")
	TObjectPtr<UX_CharacterClassInfo> CharacterClassInfo;
};
