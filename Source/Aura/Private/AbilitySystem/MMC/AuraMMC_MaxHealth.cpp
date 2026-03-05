


#include "AbilitySystem/MMC/AuraMMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/AuraCombatInterface.h"

UAuraMMC_MaxHealth::UAuraMMC_MaxHealth()
{
	// Capture the Vigor Attribute and set its capture proprieties
	VigorDefinition.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDefinition.bSnapshot = false;
	
	// Add Vigor Definition to the attributes to capture cache
	// This guarantees that |Vigor| will be captured from the |Target| at the |Time of Application|:
	// (AttributeToCapture = Vigor, AttributeSource = Target, Snapshot = false )
	RelevantAttributesToCapture.Add(VigorDefinition);
}

float UAuraMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();
	
	// Capture attribute's magnitude (value)
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTag;
	EvaluationParameters.TargetTags = TargetTag;
	
	// Get Vigor
	float Vigor = 0.0f;
	GetCapturedAttributeMagnitude(VigorDefinition,Spec,EvaluationParameters,Vigor);
	
	// Clamp value
	Vigor = FMath::Max<float>(Vigor, 0.0f);

	// Get the character's level
	int32 CharacterLevel = 1.f;
	if (IAuraCombatInterface* CombatInterface = Cast<IAuraCombatInterface>(Spec.GetContext().GetSourceObject()))
	{
		CharacterLevel = CombatInterface->GetCharacterLevel();
	}

	// Do the modifier calculation
	const float ModifierCalculation = 80.f + 2.5f * Vigor + 10.f * CharacterLevel;
	
	return ModifierCalculation;
}
