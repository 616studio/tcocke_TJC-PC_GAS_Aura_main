// Copyright © 2026 616 Studio LLC. All Rights Reserved. (https://616.studio)

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 * Native C++ Gameplay Tags declared using the Lyra extern pattern.
 * Eliminates singleton getter boilerplate and guarantees static engine-level initialization.
 */
namespace XGameplayTags
{
#pragma region Attributes
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes);

	// Primary Attributes
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Strength);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Intelligence);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Resilience);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Vigor);

	// Secondary Attributes
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_Armor);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ArmorPenetration);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_BlockChance);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitChance);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitDamage);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitResistance);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_HealthRegeneration);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ManaRegeneration);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxHealth);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxMana);

	// Vital Attributes
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Vital);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Vital_Health);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Vital_Mana);

	// Resistance Attributes
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Fire);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Ice);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Shock);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Poison);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Slashing);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Crushing);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Piercing);
#pragma endregion Attributes

#pragma region Character
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Player);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_NPC);
#pragma endregion Character

#pragma region UI & Messages
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Message);

	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Message_Item);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Message_Item_Potion);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Message_Item_Potion_Health);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Message_Item_Potion_Mana);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Message_Item_Crystal);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Message_Item_Crystal_Health);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Message_Item_Crystal_Mana);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Message_Item_Crystal_MaxHealth);

	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Message_Combat);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Message_Combat_Blocked);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Message_Combat_CriticalHit);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Message_Combat_Evaded);
#pragma endregion UI & Messages

#pragma region Input
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_LSHIFT);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_LMB);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_RMB);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_1);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_2);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_3);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_4);
#pragma endregion Input

#pragma region Gameplay Events & Abilities
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Attack);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Attack_Melee);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Attack_Projectile);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Attack_Spell);

	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_DamageType);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_DamageType_Fire);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_DamageType_Ice);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_DamageType_Shock);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_DamageType_Poison);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_DamageType_Slashing);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_DamageType_Crushing);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_DamageType_Piercing);
#pragma endregion Gameplay Events & Abilities

#pragma region Gameplay Effects
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEffect);
	GAS_AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEffect_HitReact);
#pragma endregion Gameplay Effects
}