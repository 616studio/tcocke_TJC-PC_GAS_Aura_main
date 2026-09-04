// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/X_GAS_DataTypes.h"
#include "GameFramework/PlayerState.h"
#include "X_PlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * <summary>
 * Maintains the replicated state and GAS architecture for a Player independent of their physical Pawn.
 * </summary>
 */
UCLASS()
class GAS_AURA_API AX_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
#pragma region Unreal Defaults
	
public:
	
	AX_PlayerState();
	
protected:
	
private:
	
#pragma endregion Unreal Defaults
	
#pragma region Ability System

public:
	
	/**
	 * <summary>
	 * Public getter to check the value of <c>bHasGrantedStartupData</c> to determine if this Player State has already received its persistent initial base stats and abilities from <c>X_Character_Player</c>.
	 * </summary>
	 */
	bool HasGrantedStartupData() const { return bHasGrantedStartupData; }
	
	/**
	 * <summary>
	 * Public setter for <c>bHasGrantedStartupData</c> to flag this Player State as having successfully received its startup data from <c>X_Character_Player</c>.
	 * </summary>
	 */
	void SetHasGrantedStartupData(bool bInGranted) { bHasGrantedStartupData = bInGranted; }
	
	/**
	 * <summary>
	 * Retrieves the ASC bound to this class for the Player.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Satisfies <c>IAbilitySystemInterface</c> compliance.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	/**
	 * <summary>
	 * Retrieves the Attribute Set bound to this class for the Player.
	 * </summary>
	 */
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
protected:
	
	/**
	 * <summary>
	 * ASC bound to this class for the Player.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Declared as a base pointer strictly for header decoupling (prevent unnecessary includes / header contagion).</description></item>
	 * <item><description>Instantiated as <c>UX_AbilitySystemComponent</c> to utilize our subclassed functionality (input routing, broadcasting of Gameplay Effect Asset Tags, etc.).</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/**
	 * <summary>
	 * Attribute Set bound to this class for the Player.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Declared as a base pointer strictly for header decoupling (prevent unnecessary includes / header contagion).</description></item>
	 * <item><description>Instantiated as <c>UX_AttributeSet</c> to access our subclassed <c>FGameplayAttributeData</c> properties and their network replication functions.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	
	/**
	 * <summary>
	 * Persistent guard to prevent respawning Pawns (specifically for the Player) from endlessly stacking Initialization Gameplay Effects (like Attributes) or wiping progression.
	 * </summary>
	 */
	bool bHasGrantedStartupData = false;
	
	/**
	 * <summary>
	 * Helper function to create and initialize components for the Ability System.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>WARNING:</b> This function contains <c>CreateDefaultSubobject</c> and MUST ONLY be called from the Constructor!</description></item>
	 * </list>
	 * </remarks>
	 */
	void InitComponentsForAbilitySystem();

#pragma endregion Ability System
	
#pragma region Player Character Specific
	
public:
	
	void SetPlayerCharacterLevel(const int32 NewLevel) { PlayerCharacterLevel = NewLevel; }
	
	void SetPlayerCharacterClass(const ECharacterClass NewClassType) { PlayerCharacterClass = NewClassType; }
			
protected:
	/**
	 * <summary>
	 * The Player Character's current Level.
	 * </summary>
	 */
	int32 PlayerCharacterLevel = 1;
	
	/**
	 * <summary>
	 * The Player Character's current Class.
	 * </summary>
	 */
	UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|GAS|Character Class")
	ECharacterClass PlayerCharacterClass = ECharacterClass::Warrior;
	
private:
	
#pragma endregion Player Character Specific
	
};
