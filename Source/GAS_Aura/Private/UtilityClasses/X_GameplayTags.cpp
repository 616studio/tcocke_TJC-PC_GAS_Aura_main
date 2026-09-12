// Copyright © 2026 616 Studio LLC. All Rights Reserved. (https://616.studio)

#include "GAS_Aura/UtilityClasses/X_GameplayTags.h"

namespace XGameplayTags
{
#pragma region Attributes
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes, "Attributes", "Root category for all Character Attributes.");

	// Primary Attributes
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary, "Attributes.Primary", "Primary Attribute container.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Strength, "Attributes.Primary.Strength", "Increases physical damage.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Intelligence, "Attributes.Primary.Intelligence", "Increases magical damage, Max Mana, and Mana Regeneration.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Resilience, "Attributes.Primary.Resilience", "Increases Armor and Armor Penetration.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Vigor, "Attributes.Primary.Vigor", "Increases Max Health and Health Regeneration.");

	// Secondary Attributes
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary, "Attributes.Secondary", "Secondary Attribute container.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_Armor, "Attributes.Secondary.Armor", "Reduces damage taken, improves Block Chance and Critical Hit Resistance.  Scales with Resilience by a factor of 0.25 (1 point of Armor granted for every 4 points of Resilience).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_ArmorPenetration, "Attributes.Secondary.ArmorPenetration", "Ignores a percentage of enemy Armor, increases Critical Hit Chance and Critical Hit Damage.  Scales with Resilience by a factor of 1 (1 point of Armor Penetration granted for every 1 point of Resilience).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_BlockChance, "Attributes.Secondary.BlockChance", "Chance to cut incoming damage in half.  Scales with Armor by a factor of 0.25 (1 point of Block Chance granted for every 4 points of Armor).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitChance, "Attributes.Secondary.CriticalHitChance", "Chance to inflict double damage plus a Critical Hit Damage bonus.  Scales with Armor Penetration by a factor of 0.25 (1 point of Critical Hit Chance granted for every 4 points of Armor Penetration).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitDamage, "Attributes.Secondary.CriticalHitDamage", "Bonus damage added when a Critical Hit is scored.  Scales with Armor Penetration by a factor of 1.5 (1.5 points of Critial Hit Damage granted for every 1 point of Armor Penetration).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitResistance, "Attributes.Secondary.CriticalHitResistance", "Reduces Critical Hit Chance of attacking Enemies.  Scales with Armor by a factor of 0.25 (1 point of Critical Hit Resistance granted for every 4 points of Armor).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_HealthRegeneration, "Attributes.Secondary.HealthRegeneration", "Amount of Health regenerated every 1 second.  Scales with Vigor by a factor of 0.1 (1 point of Health Regeneration granted for every 10 points of Vigor).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_ManaRegeneration, "Attributes.Secondary.ManaRegeneration", "Amount of Mana regenerated every 1 second.  Scales with Intelligence by a factor of 0.1 (1 point of Mana Regeneration granted for every 10 points of Intelligence).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_MaxHealth, "Attributes.Secondary.MaxHealth", "Maximum amount of Health obtainable.  Scales with Vigor and Character Level (Formula from X_MMC_MaxHealth:  MaxHealth = BaseValue + (VigorMultiplier * Vigor) + (LevelMultiplier * CharacterLevel).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_MaxMana, "Attributes.Secondary.MaxMana", "Maximum amount of Mana obtainable.  Scales with Intelligence and Character Level (Formula from X_MMC_MaxMana:  MaxMana = BaseValue + (IntelligenceMultiplier * Intelligence) + (LevelMultiplier * CharacterLevel).");

	// Vital Attributes
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Vital, "Attributes.Vital", "Vital Attributes container.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Vital_Health, "Attributes.Vital.Health", "Represents the Character's current value of Health out of a total maximum value derived from Max Health.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Vital_Mana, "Attributes.Vital.Mana", "Represents the Character's current value of Mana out of a total maximum value derived from Max Mana.");

	// Resistance Attributes
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Resistance, "Attributes.Resistance", "Resistance Attributes container.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Resistance_Fire, "Attributes.Resistance.Fire", "Resistance to Fire damage.  Scales with Resilience and Intelligence by a factor of 0.25 (1 point of Fire Resistance for every 4 points of Resilience plus 1 point of Fire Resistance for every 4 points of Intelligence).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Resistance_Ice, "Attributes.Resistance.Ice", "Resistance to Ice damage.  Scales with Resilience and Intelligence by a factor of 0.25 (1 point of Ice Resistance for every 4 points of Resilience plus 1 point of Ice Resistance for every 4 points of Intelligence).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Resistance_Shock, "Attributes.Resistance.Shock", "Resistance to Shock damage.  Scales with Resilience and Intelligence by a factor of 0.25 (1 point of Shock Resistance for every 4 points of Resilience plus 1 point of Shock Resistance for every 4 points of Intelligence).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Resistance_Poison, "Attributes.Resistance.Poison", "Resistance to Poison damage.  Scales with Resilience and Intelligence by a factor of 0.25 (1 point of Poison Resistance for every 4 points of Resilience plus 1 point of Poison Resistance for every 4 points of Intelligence).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Resistance_Slashing, "Attributes.Resistance.Slashing", "Resistance to Slashing damage.  Scales with Resilience and Strength by a factor of 0.25 (1 point of Slashing Resistance for every 4 points of Resilience plus 1 point of Slashing Resistance for every 4 points of Strength).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Resistance_Crushing, "Attributes.Resistance.Crushing", "Resistance to Crushing damage.  Scales with Resilience and Strength by a factor of 0.25 (1 point of Crushing Resistance for every 4 points of Resilience plus 1 point of Crushing Resistance for every 4 points of Strength).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Resistance_Piercing, "Attributes.Resistance.Piercing", "Resistance to Piercing damage.  Scales with Resilience and Strength by a factor of 0.25 (1 point of Piercing Resistance for every 4 points of Resilience plus 1 point of Piercing Resistance for every 4 points of Strength).");
#pragma endregion Attributes

#pragma region Character
	UE_DEFINE_GAMEPLAY_TAG(Character, "Character");
	UE_DEFINE_GAMEPLAY_TAG(Character_Player, "Character.Player");
	UE_DEFINE_GAMEPLAY_TAG(Character_NPC, "Character.NPC");
#pragma endregion Character

#pragma region UI & Messages
	UE_DEFINE_GAMEPLAY_TAG(UI, "UI");
	UE_DEFINE_GAMEPLAY_TAG(UI_Message, "UI.Message");

	// Item Messages
	UE_DEFINE_GAMEPLAY_TAG(UI_Message_Item, "UI.Message.Item");
	UE_DEFINE_GAMEPLAY_TAG(UI_Message_Item_Potion, "UI.Message.Item.Potion");
	UE_DEFINE_GAMEPLAY_TAG(UI_Message_Item_Potion_Health, "UI.Message.Item.Potion.Health");
	UE_DEFINE_GAMEPLAY_TAG(UI_Message_Item_Potion_Mana, "UI.Message.Item.Potion.Mana");
	UE_DEFINE_GAMEPLAY_TAG(UI_Message_Item_Crystal, "UI.Message.Item.Crystal");
	UE_DEFINE_GAMEPLAY_TAG(UI_Message_Item_Crystal_Health, "UI.Message.Item.Crystal.Health");
	UE_DEFINE_GAMEPLAY_TAG(UI_Message_Item_Crystal_Mana, "UI.Message.Item.Crystal.Mana");
	UE_DEFINE_GAMEPLAY_TAG(UI_Message_Item_Crystal_MaxHealth, "UI.Message.Item.Crystal.MaxHealth");

	// Combat Messages
	UE_DEFINE_GAMEPLAY_TAG(UI_Message_Combat, "UI.Message.Combat");
	UE_DEFINE_GAMEPLAY_TAG(UI_Message_Combat_Blocked, "UI.Message.Combat.Blocked");
	UE_DEFINE_GAMEPLAY_TAG(UI_Message_Combat_CriticalHit, "UI.Message.Combat.CriticalHit");
	UE_DEFINE_GAMEPLAY_TAG(UI_Message_Combat_Evaded, "UI.Message.Combat.Evaded");
#pragma endregion UI & Messages

#pragma region Input
	UE_DEFINE_GAMEPLAY_TAG(Input, "Input");
	UE_DEFINE_GAMEPLAY_TAG(Input_LSHIFT, "Input.LSHIFT");
	UE_DEFINE_GAMEPLAY_TAG(Input_LMB, "Input.LMB");
	UE_DEFINE_GAMEPLAY_TAG(Input_RMB, "Input.RMB");
	UE_DEFINE_GAMEPLAY_TAG(Input_1, "Input.1");
	UE_DEFINE_GAMEPLAY_TAG(Input_2, "Input.2");
	UE_DEFINE_GAMEPLAY_TAG(Input_3, "Input.3");
	UE_DEFINE_GAMEPLAY_TAG(Input_4, "Input.4");
#pragma endregion Input

#pragma region Gameplay Events & Abilities
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent, "GameplayEvent");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Attack, "GameplayEvent.Attack");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Attack_Melee, "GameplayEvent.Attack.Melee");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Attack_Projectile, "GameplayEvent.Attack.Projectile");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Attack_Spell, "GameplayEvent.Attack.Spell");

	UE_DEFINE_GAMEPLAY_TAG(Ability, "Ability");
	UE_DEFINE_GAMEPLAY_TAG(Ability_DamageType, "Ability.DamageType");
	UE_DEFINE_GAMEPLAY_TAG(Ability_DamageType_Fire, "Ability.DamageType.Fire");
	UE_DEFINE_GAMEPLAY_TAG(Ability_DamageType_Ice, "Ability.DamageType.Ice");
	UE_DEFINE_GAMEPLAY_TAG(Ability_DamageType_Shock, "Ability.DamageType.Shock");
	UE_DEFINE_GAMEPLAY_TAG(Ability_DamageType_Poison, "Ability.DamageType.Poison");
	UE_DEFINE_GAMEPLAY_TAG(Ability_DamageType_Slashing, "Ability.DamageType.Slashing");
	UE_DEFINE_GAMEPLAY_TAG(Ability_DamageType_Crushing, "Ability.DamageType.Crushing");
	UE_DEFINE_GAMEPLAY_TAG(Ability_DamageType_Piercing, "Ability.DamageType.Piercing");
#pragma endregion Gameplay Events & Abilities

#pragma region Gameplay Effects
	UE_DEFINE_GAMEPLAY_TAG(GameplayEffect, "GameplayEffect");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEffect_HitReact, "GameplayEffect.HitReact");
#pragma endregion Gameplay Effects
}