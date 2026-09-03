// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "X_GAS_DataTypes.generated.h"

// Forward declarations
class UAbilitySystemComponent;
class AActor;
class AController;
class ACharacter;
class UGameplayEffect;
class UBehaviorTree;

/**
 * <summary>
 * This file consolidates the declaration of all the different data types specific to GAS in the project.
 * </summary>
 */

/**
 * <summary>
 * Determines <b>WHEN</b> a Gameplay Effect should be applied to a target.
 * </summary>
 * <remarks>
 * <b>CALLERS:</b>
 * <list type="bullet">
 * <item><description><c>X_GameplayEffectActor_Base</c></description></item>
 * </list>
 * </remarks>
 */
UENUM(BlueprintType)
enum class EGameplayEffectApplicationPolicy : uint8
{
    ApplyOnBeginOverlap,
    ApplyOnEndOverlap,
    DoNotApply
};

/**
 * <summary>
 * Determines <b>IF</b> an infinite Gameplay Effect should be removed.
 * </summary>
 * <remarks>
 * <b>CALLERS:</b>
 * <list type="bullet">
 * <item><description><c>X_GameplayEffectActor_Base</c></description></item>
 * </list>
 * </remarks>
 */
UENUM(BlueprintType)
enum class EGameplayEffectRemovalPolicy : uint8
{
    RemoveOnEndOverlap,
    DoNotRemove
};

/**
 * <summary>
 * Helper struct to extract data from the <c>FGameplayEffectModCallbackData</c> payload received from <c>PostGameplayEffectExecute</c>.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Caches the Instigator/Source (Attacker) and Target (Victim) actor data.</description></item>
 * <item><description>Uses raw pointers over <c>TObjectPtr</c> to prevent copy constructor bloat during high-frequency callbacks.</description></item>
 * </list>
 * <b>CALLERS:</b>
 * <list type="bullet">
 * <item><description><c>X_AttributeSet</c></description></item>
 * </list>
 * </remarks>
 */
USTRUCT()
struct FX_EffectProperties
{
    GENERATED_BODY()

    FX_EffectProperties(){}

    FGameplayEffectContextHandle EffectContextHandle;
	
    /* Target: target of the Gameplay Effect (The Victim). */
    /* Source: causer of the Gameplay Effect (The Attacker). */
    UPROPERTY()
    UAbilitySystemComponent* TargetASC = nullptr;

    UPROPERTY()
    UAbilitySystemComponent* SourceASC = nullptr;

    UPROPERTY()
    AActor* TargetAvatarActor = nullptr;

    UPROPERTY()
    AActor* SourceAvatarActor = nullptr;

    UPROPERTY()
    AController* TargetController = nullptr;

    UPROPERTY()
    AController* SourceController = nullptr;

    UPROPERTY()
    ACharacter* TargetCharacter = nullptr;

    UPROPERTY()
    ACharacter* SourceCharacter = nullptr;	
};

/**
 * <summary>
 * Represents the different RPG classes that can be assigned to all Characters (Players and NPCs).
 * </summary>
 */
UENUM(BlueprintType)
enum class ECharacterClassType : uint8
{
    Elementalist,
    Warrior,
    Ranger,
    Hero
};

/**
 * <summary>
 * Helper struct to set the defaults associated with each <c>ECharacterClassType</c>.
 * </summary>
 */
USTRUCT(BlueprintType)
struct FX_CharacterClassDefaultInfo
{
    GENERATED_BODY()
    
    /**
     * <summary>
     * The specific Behavior Tree to run when this class enters Combat.
     * </summary>
     */
    UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Class Defaults|AI")
    TObjectPtr<UBehaviorTree> CombatSubTree;

    /**
     * <summary>
     * Gameplay Effect used to set the initial values of the Primary Attributes: Intelligence, Strength, Resilience, and Vigor.
     * </summary>
     */
    UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Class Defaults|Attributes")
    TSubclassOf<UGameplayEffect> PrimaryAttributes;
    
    /**
     * <summary>
     * Gameplay Effect used to update the values of the Secondary Attributes:  Armor, Armor Penetration, Block Chance, Critical Hit Chance, Critical Hit Damage, Critical Hit Resistance, Health Regeneration, Mana Regeneration, Max Health, and Max Mana.
     * </summary>
     * <remarks>
     * <b>ARCHITECTURE NOTES:</b>
     * <list type="bullet">
     * <item><description><b>Players (Hero Class):</b>The Duration Policy must be Infinite to maintain a "live link." This ensures the Secondary Attributes are updated dynamically whenever the Primary Attributes are changed.</description></item>
     * <item><description><b>NPCs:</b> Since Secondary Since NPCs do not gain levels or change their Primary Attributes during gameplay, the Duration Policy is set to Instant.</description></item>
     * <item><description>Max Health and Max Mana are derived from custom calculation classes <c>X_MMC_MaxHealth</c> and <c>X_MMC_MaxMana</c> respectively.</description></item>
     * </list>
     * </remarks>
     */
    UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Class Defaults|Attributes")
    TSubclassOf<UGameplayEffect> SecondaryAttributes;
    
    /**
     * <summary>
     * Gameplay Effect used to set the initial values of the Vital Attributes:  Health and Mana.
     * </summary>
     * <remarks>
     * <b>ARCHITECTURE NOTES:</b>
     * <list type="bullet">
     * <item><description>The values of the Vital Attributes are derived from the Secondary Attributes Max Health and Max Mana using custom calculation classes.</description></item>
     * </list>
     * </remarks>
     */
    UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Class Defaults|Attributes")
    TSubclassOf<UGameplayEffect> VitalAttributes;
    
    /**
     * <summary>
     * Gameplay Effect used to set the initial values of the Resistance Attributes:  FireResistance, IceResistance, PoisonResistance, ShockResistance, SlashingResistance, CrushingResistance, and PiercingResistance.
     * </summary>
     * <remarks>
     * <b>ARCHITECTURE NOTES:</b>
     * <list type="bullet">
     * <item><description>The values of the Resistance Attributes are derived from the Primary Attributes.</description></item>
     * </list>
     * </remarks>
     */
    UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Class Defaults|Attributes")
    TSubclassOf<UGameplayEffect> ResistanceAttributes;
    
    /**
     * <summary>
     * Infinite Gameplay Effect used to assign default Gameplay Tags to each <c>ECharacterClassType</c> that should remain indefinitely.
     * </summary>
     */
    UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Class Defaults")
    TSubclassOf<UGameplayEffect> DefaultGameplayTags;

    /**
     * <summary>
     * Default Gameplay Abilities assigned to each <c>ECharacterClassType</c>.
     * </summary>
     */
    UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Class Defaults|Abilities")
    TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

    /**
     * <summary>
     * Allows a class to start with no default abilities.
     * </summary>
     */
    UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Class Defaults|Abilities")
    bool bShouldHaveDefaultAbilities = false;
};

/**
 * <summary>
 * Helper struct to map Gameplay Tags to Anim Montages.
 * </summary>
 */
USTRUCT(BlueprintType)
struct FX_GameplayTaggedAnimMontage
{
    GENERATED_BODY()

    /**
     * <summary>
     * The AnimMontage specific to the attack behavior we want the Character to play.
     * </summary>
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UAnimMontage> AnimMontage;

    /**
     * <summary>
     * The Gameplay Tag to identify the socket associated with the AnimMontage that will be used to apply damage.
     * </summary>
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag SocketLocationTag;

    /**
     * <summary>
     * The Gameplay Tag to identify the type of attack being performed (melee, projectile, spell).
     * </summary>
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag AttackType;
    
    /**
     * <summary>
     * Time in seconds from the start of the Montage until the attack physically hits.
     * </summary>
     * <remarks>
     * <b>ARCHITECTURE NOTES:</b>
     * <list type="bullet">
     * <item><description><b>Server Optimization:</b> Replaces AnimNotifies for server-side combat calculation, allowing the server to disable skeletal mesh ticking entirely for off-screen enemies to save massive CPU overhead.</description></item>
     * <item><description>Used by <c>UAbilityTask_WaitDelay</c> in the Gameplay Ability to trigger the damage payload.</description></item>
     * </list>
     * </remarks>
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float ImpactDelay = 0.5f;
};


