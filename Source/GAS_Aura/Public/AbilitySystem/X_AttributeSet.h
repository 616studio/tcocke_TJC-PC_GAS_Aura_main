// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GAS_Aura/UtilityClasses/X_CustomDataTypes.h"
#include "AttributeSet.h"
#include "X_AttributeSet.generated.h"

struct FX_EffectProperties;
/**
 * <summary>
 * GAS requires subclassing of <c>UAttributeSet</c> in order to define, clamp, and replicate the project's specific Gameplay Attributes.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Intercepts incoming Gameplay Effects (via <c>PreAttributeChange</c>, <c>PreAttributeBaseChange</c>, and <c>PostGameplayEffectExecute</c>) to clamp values or trigger additional logic.</description></item>
 * </list>
 * <b>IMPORTANT:</b>
 * <list type="bullet">
 * <item><description>Do <b>NOT</b> modify Attributes directly via standard C++ assignment. <b>ALWAYS</b> use Gameplay Effects to preserve prediction and replication pipelines.</description></item>
 * </list>
 * </remarks>
 */
UCLASS()
class GAS_AURA_API UX_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	
	UX_AttributeSet();
	
	/**
	* <summary>
	* Intercepts temporary modifications (Duration/Infinite Gameplay Effects) to an Attribute's Current Value before they are applied.
	* </summary>
	* <remarks>
	* <b>ARCHITECTURE NOTES:</b>
	* <list type="bullet">
	* <item><description>Used <b>STRICTLY</b> for mathematical clamping by modifying the <c>NewValue</c> reference.</description></item>
	* <item><description><b>NO</b> gameplay logic (death, spawning) should be used here, since GAS may execute this multiple times purely for Client prediction or UI queries without actually applying the Gameplay Effect.</description></item>
	* </list>
	* </remarks>
	* <param name="Attribute">[<c>FGameplayAttribute&</c>]:  The Attribute to be modified.</param>
	* <param name="NewValue">[<c>float&</c>]:  The proposed new value of the Attribute.</param>
	*/
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/**
	 * <summary>
	 * Intercepts permanent modifications (Instant/Periodic Gameplay Effects) to an Attribute's Base Value before they are applied.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Used <b>STRICTLY</b> for mathematical clamping by modifying the <c>NewValue</c> reference.</description></item>
	 * <item><description><b>NO</b> gameplay logic (death, spawning) should be used here, since GAS may execute this multiple times purely for Client prediction or UI queries without actually applying the Gameplay Effect.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="Attribute">[<c>FGameplayAttribute&</c>]: The Attribute to be modified.</param>
	 * <param name="NewValue">[<c>float&</c>]: The proposed new value of the Attribute.</param>
	 */
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	/**
	* <summary>
	* Fires immediately <b>AFTER</b> the engine finishes applying a Gameplay Effect's modifiers to an Attribute's Base Value.
	* </summary>
	* <remarks>
	* <b>ARCHITECTURE NOTES:</b>
	* <list type="bullet">
	* <item><description>We use <c>SetEffectProperties</c> to extract the relevant Source and Target data from the <c>FGameplayEffectModCallbackData</c> (<b>Data</b>) payload, giving us the exact context needed to calculate damage mitigations and trigger visual hit-reacts.</description></item>
	* <item><description><b>ONLY</b> fires for Instant & Periodic effects (changes to the permanent Base Value).</description></item>
	* <item><description><b>DOES NOT</b> fire for Duration & Infinite effects (temporary Current Value changes are automatically handled by the ASC's <c>FAggregator</c>).</description></item>
	* </list>
	* <b>PRIMARY RESPONSIBILITIES:</b>
	* <list type="bullet">
	* <item><description><b>"Meta-Attribute" Processing:</b> take the accumulated "IncomingDamage", manually subtract it from "Health", and reset IncomingDamage to 0.</description></item>
	* <item><description><b>Clamping:</b> ensure changed Attributes stay within specified bounds (e.g., Health doesn't exceed MaxHealth).</description></item>
	* <item><description><b>State Checks (Death Logic):</b> check if custom math caused Health to hit 0, and trigger death animations/logic.</description></item>
	* <item><description><b>Visuals and Events:</b> spawn Floating Combat Text, perform hit-reacts, or trigger reactive passives (ex: "Thorns" aura).</description></item>
	* </list>
	* <b>IMPORTANT:</b>
	* <list type="bullet">
	* <item><description>We avoid applying damage directly to the Health attribute via Gameplay Effects. Instead, we use a non-replicating Attribute ("Meta-Attribute") called <c>IncomingDamage</c>.</description></item>
	* <item><description><b>Why?</b>  Since "Meta-Attributes" are not replicated, we can use them as a temporary "buffer."</description></item>  
	* <item><description>This prevents replication jitter and allows you to easily calculate damage reductions (Armor, Shields) in one centralized C++ location before the actual Health value is touched.</description></item>
	* </list>
	* </remarks>
	* <param name="Data">[<c>FGameplayEffectModCallbackData&</c>]: Handed to us by GAS after a Gameplay Effect executes.</param>
	*/
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	/**
    * <summary>
    * Used to define exactly which Gameplay Attributes the Server must synchronize with the Clients over the network.
    * </summary>
    * <remarks>
    * <b>ARCHITECTURE NOTES:</b>
    * <list type="bullet">
    * <item><description>Requires <b>Super::GetLifetimeReplicatedProps(OutLifetimeProps)</b> to be called at the top of this function.</description></item>
    * <item><description>Uses the <b>DOREPLIFETIME_CONDITION_NOTIFY</b> macro to establish the specific replication rules for each individual Attribute.</description></item>
    * </list>
    * <b>PRIMARY RESPONSIBILITY:</b>
    * <list type="bullet">
    * <item><description><b>Registration:</b> tells the Unreal network engine to actively track every Gameplay Attribute so Players can see their own (and others') correct stats.</description></item>
    * </list>
    * <b>IMPORTANT:</b>
    * <list type="bullet">
    * <item><description>Standard Unreal replication usually uses <c>REPNOTIFY_OnChanged</c>.  However, in GAS, we <b>MUST</b> use <c>REPNOTIFY_Always</c> for Attributes.</description></item>
	 * <item><description><b>Why?</b>  GAS uses Client Prediction, which means a client might falsely predict taking 50 damage and proceed to drop their local Health.</description></item>  
	 * <item><description>If the Server rejects that damage, the Server's Health value never actually changed.  If we used "OnChanged", the Server would stay silent, and the Client's Health would remain permanently desynced!</description></item>  
	 * <item><description><c>REPNOTIFY_Always</c> forces the Server to overwrite the Client's math even if the Server's value remained identical.</description></item>
	 * </list>
    * </remarks>
    * <param name="OutLifetimeProps">[<c>TArray&</c>(<c>FLifetimeProperty</c>)]:  This class is used to track properties marked to be replicated.</param>
    */
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
#pragma region Vital Attributes
	
#pragma region Health
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "***CUSTOM|GAS|Attributes|Vital")
	FGameplayAttributeData Health;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& PreviousHealth);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Health);

#pragma endregion Health

#pragma region Mana
		
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "***CUSTOM|GAS|Attributes|Vital")
	FGameplayAttributeData Mana;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& PreviousMana);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Mana);

#pragma endregion Mana
	
#pragma endregion Vital Attributes

#pragma region Primary Attributes
	
#pragma region Strength
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "***CUSTOM|GAS|Attributes|Primary")
	FGameplayAttributeData Strength;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& PreviousStrength);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Strength);

#pragma endregion Strength

#pragma region Intelligence

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "***CUSTOM|GAS|Attributes|Primary")
	FGameplayAttributeData Intelligence;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& PreviousIntelligence);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Intelligence);

#pragma endregion Intelligence

#pragma region Resilience

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "***CUSTOM|GAS|Attributes|Primary")
	FGameplayAttributeData Resilience;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& PreviousResilience);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Resilience);

#pragma endregion Resilience

#pragma region Vigor

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "***CUSTOM|GAS|Attributes|Primary")
	FGameplayAttributeData Vigor;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& PreviousVigor);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Vigor);

#pragma endregion Vigor

#pragma endregion Primary Attributes

#pragma region Secondary Attributes

#pragma region Armor

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "***CUSTOM|GAS|Attributes|Secondary")
	FGameplayAttributeData Armor;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& PreviousArmor);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Armor);

#pragma endregion Armor

#pragma region Armor Penetration

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "***CUSTOM|GAS|Attributes|Secondary")
	FGameplayAttributeData ArmorPenetration;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& PreviousArmorPenetration);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, ArmorPenetration);

#pragma endregion Armor Penetration

#pragma region Block Chance

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "***CUSTOM|GAS|Attributes|Secondary")
	FGameplayAttributeData BlockChance;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& PreviousBlockChance);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, BlockChance);

#pragma endregion Block Chance

#pragma region Critical Hit Chance

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "***CUSTOM|GAS|Attributes|Secondary")
	FGameplayAttributeData CriticalHitChance;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& PreviousCriticalHitChance);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, CriticalHitChance);

#pragma endregion Critical Hit Chance

#pragma region Critical Hit Damage

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "***CUSTOM|GAS|Attributes")
	FGameplayAttributeData CriticalHitDamage;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& PreviousCriticalHitDamage);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, CriticalHitDamage);

#pragma endregion Critical Hit Damage
	
#pragma region Critical Hit Resistance

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "***CUSTOM|GAS|Attributes")
	FGameplayAttributeData CriticalHitResistance;

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& PreviousCriticalHitResistance);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, CriticalHitResistance);

#pragma endregion Critical Hit Resistance

#pragma region Health Regeneration

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "***CUSTOM|GAS|Attributes")
	FGameplayAttributeData HealthRegeneration;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& PreviousHealthRegeneration);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, HealthRegeneration);

#pragma endregion Health Regeneration

#pragma region Mana Regeneration

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "***CUSTOM|GAS|Attributes")
	FGameplayAttributeData ManaRegeneration;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& PreviousManaRegeneration);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, ManaRegeneration);

#pragma endregion Mana Regeneration
	
#pragma region MaxMana
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "***CUSTOM|GAS|Attributes")
	FGameplayAttributeData MaxMana;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& PreviousMaxMana);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, MaxMana);

#pragma endregion MaxMana

#pragma region MaxHealth

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "***CUSTOM|GAS|Attributes")
	FGameplayAttributeData MaxHealth;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& PreviousMaxHealth);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, MaxHealth);

#pragma endregion MaxHealth

#pragma endregion Secondary Attributes	
	
#pragma region Resistance Attributes
	
#pragma region Fire Resistance
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "***CUSTOM|GAS|Attributes")
	FGameplayAttributeData FireResistance;

	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& PreviousFireResistance);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, FireResistance);
	
#pragma endregion FireResistance
	
#pragma region Ice Resistance
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IceResistance, Category = "***CUSTOM|GAS|Attributes")
	FGameplayAttributeData IceResistance;

	UFUNCTION()
	void OnRep_IceResistance(const FGameplayAttributeData& PreviousIceResistance);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, IceResistance);
	
#pragma endregion Ice Resistance
	
#pragma region Poison Resistance
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PoisonResistance, Category = "***CUSTOM|GAS|Attributes")
	FGameplayAttributeData PoisonResistance;

	UFUNCTION()
	void OnRep_PoisonResistance(const FGameplayAttributeData& PreviousPoisonResistance);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, PoisonResistance);
	
#pragma endregion Poison Resistance
	
#pragma region Shock Resistance
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ShockResistance, Category = "***CUSTOM|GAS|Attributes")
	FGameplayAttributeData ShockResistance;

	UFUNCTION()
	void OnRep_ShockResistance(const FGameplayAttributeData& PreviousShockResistance);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, ShockResistance);
	
#pragma endregion Shock Resistance
	
#pragma region Slashing Resistance
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SlashingResistance, Category = "***CUSTOM|GAS|Attributes")
	FGameplayAttributeData SlashingResistance;

	UFUNCTION()
	void OnRep_SlashingResistance(const FGameplayAttributeData& PreviousSlashingResistance);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, SlashingResistance);
	
#pragma endregion Slashing Resistance
	
#pragma region Crushing Resistance
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CrushingResistance, Category = "***CUSTOM|GAS|Attributes")
	FGameplayAttributeData CrushingResistance;

	UFUNCTION()
	void OnRep_CrushingResistance(const FGameplayAttributeData& PreviousCrushingResistance);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, CrushingResistance);
	
#pragma endregion Crushing Resistance
	
#pragma region Piercing Resistance
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PiercingResistance, Category = "***CUSTOM|GAS|Attributes")
	FGameplayAttributeData PiercingResistance;

	UFUNCTION()
	void OnRep_PiercingResistance(const FGameplayAttributeData& PreviousPiercingResistance);
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, PiercingResistance);
	
#pragma endregion Piercing Resistance
	
#pragma endregion Resistance Attributes
	
#pragma region Meta Attributes
	
#pragma region IncomingDamage

	UPROPERTY(BlueprintReadOnly, Category = "***CUSTOM|GAS|Attributes")
	FGameplayAttributeData IncomingDamage;
	
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, IncomingDamage);
	
#pragma endregion IncomingDamage
	
#pragma endregion Meta Attributes
	
private:
	
	/**
	 * <summary>
	 * Helper function to extract all relevant data from the <c>EffectContextHandle</c> (provided by the <c>FGameplayEffectModCallbackData</c> payload) in order to populate our custom <c>FX_EffectProperties</c> payload.
	 * </summary> 
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Called from <b>PostGameplayEffectExecute</b> to cache the <c>EffectContextHandle</c> and the Source/Target ASC, AvatarActor, Controller, and Character.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="Data">[<c>FGameplayEffectModCallbackData&</c>]: Handed to us by GAS after a Gameplay Effect executes.</param>
	 * <param name="EffectProperties">[<c>FX_EffectProperties&</c>]:  The custom payload we are passing in by reference to be populated.</param>
	*/
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FX_EffectProperties& EffectProperties);
};
