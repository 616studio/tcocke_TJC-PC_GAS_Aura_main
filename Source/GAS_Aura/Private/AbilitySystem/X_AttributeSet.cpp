// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))


#include "AbilitySystem/X_AttributeSet.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Characters/X_Character_Base.h"
#include "Net/UnrealNetwork.h"

UX_AttributeSet::UX_AttributeSet()
{
}

void UX_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{ 
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
}

void UX_AttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
}

void UX_AttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UX_AttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	// Clamps current Health down to new MaxHealth when MaxHealth changes result in Health being larger than MaxHealth.
	if (Attribute == GetMaxHealthAttribute())
	{
		if (GetHealth() > NewValue)
		{
			SetHealth(NewValue);
		}
	}
	
	// Clamps current Mana down to new MaxMana when MaxMana changes result in Mana being larger than MaxMana.
	if (Attribute == GetMaxManaAttribute())
	{
		if (GetMana() > NewValue)
		{
			SetMana(NewValue);
		}
	}
}

void UX_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Extract all the relevant info we need from the Data payload.
	FX_EffectProperties EffectProperties;
	SetEffectProperties(Data, EffectProperties);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}

	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}	
	
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		// We always want to store the value of the IncomingDamage Attribute in a temporary value so we can clear out the IncomingDamage Attribute value between every application.
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.0f);
		
		/*
		// Check if they are already dead BEFORE applying damage to prevent duplication of death behavior (ragdoll, material dissolve, etc.).
		bool bWasAlreadyDead = false;
		IX_CharacterInterface* CharacterInterface = Cast<IX_CharacterInterface>(EffectProperties.TargetAvatarActor);
		if (CharacterInterface)
		{
			bWasAlreadyDead = CharacterInterface->Execute_IsDead(EffectProperties.TargetAvatarActor);
		}
		*/
		
		/*
		// Route visuals through the TargetAvatarActor, not the TargetCharacter (which relies on the Controller) so non-Controller based actors can still spawn floating combat text.
		// We ALWAYS broadcast the damage event so the UI (spawned floating combat text) can show "0", "Blocked", or "Immune".
		if (AX_Character_Base* TargetAvatar = Cast<AX_Character_Base>(EffectProperties.TargetAvatarActor))
		{
			// Create empty tags to be used to extract the actual Gameplay Tags from the Effect Context.
			FGameplayTag MessageTag = FGameplayTag::EmptyTag;
			FGameplayTag AbilityTag = FGameplayTag::EmptyTag;
				
			// Message Tag
			if (UX_AbilitySystemLibrary::IsBlockedHit(EffectProperties.EffectContextHandle))
			{
				MessageTag = X_GameplayTags::UI_Message_Combat_Blocked; 
			}
			else if (UX_AbilitySystemLibrary::IsCriticalHit(EffectProperties.EffectContextHandle))
			{
				MessageTag = XGameplayTags::UI_Message_Combat_CriticalHit;
			}
						
			// Ability Icon Tag
			// Grab all Gameplay Tags attached to this Gameplay Effect.
			FGameplayTagContainer AssetTags;
			Data.EffectSpec.GetAllAssetTags(AssetTags);

			// Loop through the Gameplay Tags to find the one representing the Ability's Damage Type.
			for (const FGameplayTag& Tag : AssetTags)
			{
				if (Tag.MatchesTag(XGameplayTags::Ability_DamageType))
				{
					AbilityTag = Tag;
					break; 
				}
			}

			// Broadcast the damage for the UI of all clients.
			TargetAvatar->MulticastRPC_BroadcastDamage(
				LocalIncomingDamage, 
				TargetAvatar->GetActorLocation(), 
				EffectProperties.SourceAvatarActor, 
				MessageTag, 
				AbilityTag
			);
			
		}
		*/
		
		/*
		// If they were already dead, or no actual damage was dealt to health, we can safely exit now
		// before doing health subtractions or triggering hit reaction animations.
		if (bWasAlreadyDead || LocalIncomingDamage <= 0.0f) return;
		*/
		
		// Update Health with clamping.
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
		
		/*
		// Check if this application instance is fatal for the Character.
		const bool bIsFatal = NewHealth <= 0.0f;
		
		// Perform death behavior.
		if (bIsFatal)
		{
			// TODO:  Implement!
			// Construct an FGameplayEventData payload containing full context attribution
			// This allows abilities (like a passive "Soul Steal" or XP calculation ability) to listen for the event on the Source ASC without polluting the UAttributeSet with hardcoded class dependencies.
			FGameplayEventData EventData;
			EventData.Instigator = EffectProperties.SourceAvatarActor; // Attacker / Hazard
			EventData.Target = EffectProperties.TargetAvatarActor;     // Victim
			EventData.OptionalObject = EffectProperties.EffectContextHandle.GetSourceObject(); // Weapon / Ability Data
			EventData.EventMagnitude = LocalIncomingDamage;
			
			// A. Notify Target ASC of Death via Gameplay Event
			if (EffectProperties.TargetASC)
			{
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
				   EffectProperties.TargetAvatarActor, 
				   TAG_Event_Unit_Death, 
				   EventData
				);
			}
			
			// B. Award Kill Credit / XP / Aggro Removal to Source ASC (If valid and not self-inflicted)
			// Safely handles environmental hazards (spike traps, lava) where Props.SourceASC is nullptr, as well as suicide/fall damage where Source equals Target.
			if (EffectProperties.SourceASC && EffectProperties.SourceAvatarActor != EffectProperties.TargetAvatarActor)
			{
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
				   EffectProperties.SourceAvatarActor, 
				   TAG_Event_Unit_KillCredit, 
				   EventData
				);
			}
			
			if (CharacterInterface)
			{
				CharacterInterface->PerformDeathBehavior();
			}
		}
		
		// Perform HitReact behavior.
		else
		{
			
			// Trigger the target's Hit React Gameplay Ability by searching their granted abilities for the HitReact Asset Tag.
			if (EffectProperties.TargetASC)
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(TAG_Custom_GameplayEffect_Modifier_HitReact);
				EffectProperties.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
		}	
		*/
	}
}


void UX_AttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Mana, COND_None, REPNOTIFY_Always);

	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Vigor, COND_None, REPNOTIFY_Always);

	// Secondary Attributes	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxMana, COND_None, REPNOTIFY_Always);
	
	// Resistance Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, IceResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ShockResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, PoisonResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, SlashingResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CrushingResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, PiercingResistance, COND_None, REPNOTIFY_Always);
	
}

void UX_AttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,
                                          FX_EffectProperties& EffectProperties)
{
	/*
	 * TERMINOLOGY & DATA EXTRACTION GUIDE:
	 * * SOURCE (The Attacker / Instigator): The entity that originally created and sent this Gameplay Effect.
	 * * TARGET (The Defender / Victim): The entity receiving the Gameplay Effect.
	 */ 
     	
	if (!Data.EffectSpec.GetContext().IsValid()) return;
	
	// Store the Effect Context Handle.
	EffectProperties.EffectContextHandle = Data.EffectSpec.GetContext();
	
	// Extract the SOURCE data.
	EffectProperties.SourceASC = EffectProperties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(EffectProperties.SourceASC) && EffectProperties.SourceASC->AbilityActorInfo.IsValid() && EffectProperties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.SourceAvatarActor = EffectProperties.SourceASC->AbilityActorInfo->AvatarActor.Get();
		EffectProperties.SourceController = EffectProperties.SourceASC->AbilityActorInfo->PlayerController.Get();

		// If the SourceController does not belong to a Player, try to get the Controller on the Pawn instead.
		if (EffectProperties.SourceController == nullptr && EffectProperties.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(EffectProperties.SourceAvatarActor))
			{
				EffectProperties.SourceController = Pawn->GetController();
			}
		}

		// If we were able to find a Controller, try and get the Character from its Pawn.
		if (EffectProperties.SourceController)
		{
			EffectProperties.SourceCharacter = Cast<ACharacter>(EffectProperties.SourceController->GetPawn());
		}
	}

	// Extract the TARGET data.
	// The Target ASC is always the ASC that owns this AttributeSet.
	// It is guaranteed valid inside PostGameplayEffectExecute, so we assign it unconditionally.
	// However, the AvatarActor may be invalid (ex: if the Character was just destroyed this frame), which is why we check it safely below.
	EffectProperties.TargetASC = &Data.Target;

	// Try to get all the Target data.
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		EffectProperties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();

		// If the TargetController does not belong to a Player, try to get the Controller on the Pawn instead.
		if (EffectProperties.TargetController == nullptr && EffectProperties.TargetAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(EffectProperties.TargetAvatarActor))
			{
				EffectProperties.TargetController = Pawn->GetController();
			}
		}

		// If we were able to find a Controller, try and get the Character from its Pawn.
		if (EffectProperties.TargetController)
		{
			EffectProperties.TargetCharacter = Cast<ACharacter>(EffectProperties.TargetController->GetPawn());
		}
	}
}


#pragma region Vital Attributes

void UX_AttributeSet::OnRep_Health(const FGameplayAttributeData& PreviousHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, PreviousHealth);	
}

void UX_AttributeSet::OnRep_Mana(const FGameplayAttributeData& PreviousMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Mana, PreviousMana);
}

#pragma endregion Vital Attributes

#pragma region Primary Attributes

void UX_AttributeSet::OnRep_Strength(const FGameplayAttributeData& PreviousStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Strength, PreviousStrength);
}

void UX_AttributeSet::OnRep_Intelligence(const FGameplayAttributeData& PreviousIntelligence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Intelligence, PreviousIntelligence);
}

void UX_AttributeSet::OnRep_Resilience(const FGameplayAttributeData& PreviousResilience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Resilience, PreviousResilience);
}

void UX_AttributeSet::OnRep_Vigor(const FGameplayAttributeData& PreviousVigor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Vigor, PreviousVigor);
}

#pragma endregion Primary Attributes

#pragma region Secondary Attributes

void UX_AttributeSet::OnRep_Armor(const FGameplayAttributeData& PreviousArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Armor, PreviousArmor);
}

void UX_AttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& PreviousArmorPenetration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ArmorPenetration, PreviousArmorPenetration);
}

void UX_AttributeSet::OnRep_BlockChance(const FGameplayAttributeData& PreviousBlockChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, BlockChance, PreviousBlockChance);	
}

void UX_AttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& PreviousCriticalHitChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitChance, PreviousCriticalHitChance);
}

void UX_AttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& PreviousCriticalHitDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitDamage, PreviousCriticalHitDamage);
}

void UX_AttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& PreviousCriticalHitResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitResistance, PreviousCriticalHitResistance);
}

void UX_AttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& PreviousHealthRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, HealthRegeneration, PreviousHealthRegeneration);
}

void UX_AttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& PreviousManaRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ManaRegeneration, PreviousManaRegeneration);
}

void UX_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& PreviousMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, PreviousMaxHealth);
}

void UX_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& PreviousMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxMana, PreviousMaxMana);
}

#pragma endregion Secondary Attributes

#pragma region Resistance Attributes

void UX_AttributeSet::OnRep_FireResistance(const FGameplayAttributeData& PreviousFireResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, FireResistance, PreviousFireResistance);
}

void UX_AttributeSet::OnRep_IceResistance(const FGameplayAttributeData& PreviousIceResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, IceResistance, PreviousIceResistance);
}

void UX_AttributeSet::OnRep_PoisonResistance(const FGameplayAttributeData& PreviousPoisonResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, PoisonResistance, PreviousPoisonResistance);
}

void UX_AttributeSet::OnRep_ShockResistance(const FGameplayAttributeData& PreviousShockResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ShockResistance, PreviousShockResistance);
}

void UX_AttributeSet::OnRep_SlashingResistance(const FGameplayAttributeData& PreviousSlashingResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, SlashingResistance, PreviousSlashingResistance);
}

void UX_AttributeSet::OnRep_CrushingResistance(const FGameplayAttributeData& PreviousCrushingResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CrushingResistance, PreviousCrushingResistance);
}

void UX_AttributeSet::OnRep_PiercingResistance(const FGameplayAttributeData& PreviousPiercingResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, PiercingResistance, PreviousPiercingResistance);
}

#pragma endregion Resistance Attributes
