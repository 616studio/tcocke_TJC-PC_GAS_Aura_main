// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilitySystem/X_GAS_DataTypes.h"
#include "X_CharacterClassInfo.generated.h"


// Forward declarations
class UGameplayEffect;

/**
 * <summary>
 * Data Asset stored as <c>DA_CharacterClassInfo</c> that contains the default Character specific info for each <c>ECharacterClassType</c>.
 * </summary>
 */
UCLASS()
class GAS_AURA_API UX_CharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/**
	 * <summary>
	 * Maps each <c>ECharacterClassType</c> to its default values.
	 * </summary>
	 */
	UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Classes")
	TMap<ECharacterClassType, FX_CharacterClassDefaultInfo> CharacterClassInformation;

	/**
	 * <summary>
	 * Gameplay Abilities shared between all default class types.
	 * </summary>
	 */
	UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Shared Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> SharedGameplayAbilities;
	
	/**
	 * <summary>
	 * Provides a way to scale specific Attributes that are used as part of the final damage calculation done to a target.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>ARMOR PENETRATION:</b> The higher the level, the more points of Armor Penetration it requires to reduce the same amount of Effective Armor by 1%.</description></item>
	 * <item><description><b>EFFECTIVE ARMOR:</b> The higher the level, the more points of Armor it requires to increase the same amount of Effective Armor that ignores IncomingDamage by 1%.</description></item>
	 * <item><description><b>CRITICAL HIT RESISTANCE:</b> The higher the level, the more points of Critical Hit Resistance to decrease the same amount of Critical Hit Chance by 1%.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Shared Class Defaults")
	TObjectPtr<UCurveTable> DamageCalcCoefficients;

	/**
	 * <summary>
	 * Returns the <c>FX_CharacterClassDefaultInfo</c> for the specified <c>ECharacterClassType</c>.
	 * </summary>
	 * 
	 */	
	FX_CharacterClassDefaultInfo& GetCharacterClassDefaultInfo(ECharacterClassType CharacterClassType);
	
	/**
	 * <summary>
	 * Maps an incoming Damage Type Gameplay Tag to its corresponding Attribute Resistance Gameplay Tag.
	 * </summary>
	 * <remarks>
	 * <list type="bullet">
	 * <item><description>Configured in the Editor so designers can easily add new elemental damage types without touching C++.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
};
