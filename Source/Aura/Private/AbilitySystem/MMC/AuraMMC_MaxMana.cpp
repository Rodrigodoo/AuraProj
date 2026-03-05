


#include "AbilitySystem/MMC/AuraMMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/AuraCombatInterface.h"

UAuraMMC_MaxMana::UAuraMMC_MaxMana()
{
	// Capture the Intelligence Attribute and set its capture proprieties
	IntelligenceDefinition.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDefinition.bSnapshot = false;
	
	// Add Intelligence Definition to the attributes to capture cache
	// This guarantees that |Intelligence| will be captured from the |Target| at the |Time of Application|:
	// (AttributeToCapture = Intelligence, AttributeSource = Target, Snapshot = false )
	RelevantAttributesToCapture.Add(IntelligenceDefinition);
}

float UAuraMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();
	
	// Capture attribute's magnitude (value)
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTag;
	EvaluationParameters.TargetTags = TargetTag;
	
	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDefinition, Spec, EvaluationParameters,Intelligence);
	
	// Clamp value
	Intelligence = FMath::Max<float>(Intelligence, 0.f);
	
	// Get the character's level
	float CharacterLevel = 1.f;
	if (IAuraCombatInterface* CombatInterface = Cast<IAuraCombatInterface>(Spec.GetContext().GetSourceObject()))
	{
		CharacterLevel = CombatInterface->GetCharacterLevel();
	}
	
	// Do the modifier calculation
	const float ModificationCalculation = 50.f + Intelligence * 2.5 + CharacterLevel;
	
	return ModificationCalculation;
}
