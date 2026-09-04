// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#include "ModMagCalcs/X_MMC_MaxHealth.h"
#include "AbilitySystem/X_AttributeSet.h"
#include "Interfaces/X_CharacterInterface.h"

UX_MMC_MaxHealth::UX_MMC_MaxHealth()
{
	// ATTRIBUTE CAPTURE PROCESS:
	// 1. Specify which Attribute we want to capture from a specific Attribute Set.
	// 2. Capture the Attribute from either the Target or Source of the Gameplay Effect this Exec Calc is assigned to.
	// 3. Decide if you want the 'snapshot' value of the Attribute or not (TRUE: the value of the Attribute when the Effect Spec gets created; FALSE: the value of the Attribute when the Effect Spec gets applied).
	// 4. Add the fully configured Definition to the Exec Calc's internal list of required captures.
	
	TargetVigorDefinition.AttributeToCapture = UX_AttributeSet::GetVigorAttribute();
	TargetVigorDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	TargetVigorDefinition.bSnapshot = false;
	RelevantAttributesToCapture.Add(TargetVigorDefinition);
}

float UX_MMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather all active Gameplay Tags from both the Source (Attacker/Caster) and the Target.
	// While this particular MMC formula itself doesn't explicitly rely on Gameplay Tags, the underlying Attribute might.
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// We pass the captured tags into the Aggregator so it knows exactly which conditional Attribute modifiers to apply (or ignore) when calculating the final magnitude of the Attribute.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Retrieve the actual numerical value of the captured Attribute.
	float Vigor = 0.0f;	
	GetCapturedAttributeMagnitude(TargetVigorDefinition, Spec, EvaluationParameters, Vigor);
	
	// Clamp the value to ensure the Attribute is never a negative number, which would break the math.
	Vigor = FMath::Max<float>(Vigor, 0.0f);

	// Retrieve the Character's current Level by ensuring the Source of the Gameplay Effect implements X_CharacterInterface.
	int32 CharacterLevel = 1;
	UObject* SourceObject = Spec.GetContext().GetSourceObject();
	AActor* InstigatorActor = Spec.GetContext().GetInstigator();
	
	// Fallback logic: Try the explicitly assigned Source Object first. 
	// If it fails (or is null), try the broader Instigator of the effect.
	if (IX_CharacterInterface* SourceInterface = Cast<IX_CharacterInterface>(SourceObject))
	{
		CharacterLevel = SourceInterface->GetCharacterLevel();
	}
	else if (IX_CharacterInterface* InstigatorInterface = Cast<IX_CharacterInterface>(InstigatorActor))
	{
		CharacterLevel = InstigatorInterface->GetCharacterLevel();
	}

	return BaseValue + (VigorMultiplier * Vigor) + (LevelMultiplier * CharacterLevel);
	
}
