// Copyright © 2026 616 Studio LLC. All Rights Reserved. (https://616.studio)

#include "X_GameplayTags.h"

namespace XGameplayTags
{
#pragma region Attributes
	UE_DEFINE_GAMEPLAY_TAG(Attributes, "Attributes");

	// Primary Attributes
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Primary, "Attributes.Primary");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Primary_Strength, "Attributes.Primary.Strength");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Primary_Intelligence, "Attributes.Primary.Intelligence");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Primary_Resilience, "Attributes.Primary.Resilience");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Primary_Vigor, "Attributes.Primary.Vigor");

	// Secondary Attributes
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary, "Attributes.Secondary");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_Armor, "Attributes.Secondary.Armor");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_ArmorPenetration, "Attributes.Secondary.ArmorPenetration");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_BlockChance, "Attributes.Secondary.BlockChance");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_CriticalHitChance, "Attributes.Secondary.CriticalHitChance");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_CriticalHitDamage, "Attributes.Secondary.CriticalHitDamage");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_CriticalHitResistance, "Attributes.Secondary.CriticalHitResistance");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_HealthRegeneration, "Attributes.Secondary.HealthRegeneration");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_ManaRegeneration, "Attributes.Secondary.ManaRegeneration");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_MaxHealth, "Attributes.Secondary.MaxHealth");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_MaxMana, "Attributes.Secondary.MaxMana");

	// Vital Attributes
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Vital, "Attributes.Vital");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Vital_Health, "Attributes.Vital.Health");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Vital_Mana, "Attributes.Vital.Mana");

	// Resistance Attributes
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance, "Attributes.Resistance");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance_Fire, "Attributes.Resistance.Fire");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance_Ice, "Attributes.Resistance.Ice");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance_Shock, "Attributes.Resistance.Shock");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance_Poison, "Attributes.Resistance.Poison");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance_Slashing, "Attributes.Resistance.Slashing");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance_Crushing, "Attributes.Resistance.Crushing");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance_Piercing, "Attributes.Resistance.Piercing");
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