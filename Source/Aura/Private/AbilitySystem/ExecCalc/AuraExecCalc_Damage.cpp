


#include "AbilitySystem/ExecCalc/AuraExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTagsManager.h"
#include "AbilitySystem/AuraAttributeSet.h"

// Internal struct to store Attribute Capture Definitions
struct AuraDamageStatics
{
	// Attribute Capture Declarations 
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	
	AuraDamageStatics()
	{
		// Attribute Capture Definitions 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
	}
};

// Get the Damage Statics object
static const AuraDamageStatics& DamageStatics()
{
	// It will always be the same object
	static AuraDamageStatics DStatics;
	return DStatics;
}

UAuraExecCalc_Damage::UAuraExecCalc_Damage()
{
	// Add Attributes to Capture from the Damage Statics
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
}

void UAuraExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	
	const AActor* SourceAvatar = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;
	
	// Get the Effect Spect
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTag = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = EffectSpec.CapturedTargetTags.GetAggregatedTags();
	
	// Capture attribute's magnitude (value)
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTag;
	EvaluationParameters.TargetTags = TargetTag;
	
	/*
	 * Damage Calculation - Block Chance -> Armor
	 */
	
	// Get Damage Set by Caller Magnitude (Damage coming from Source)
	float Damage = EffectSpec.GetSetByCallerMagnitude(FAuraGameplayTagsManager::Get().Damage);
	
	//~ Block Chance
	
	// Capture Block Chance on Target, and determine if there was a successful Block
	float TargetBlockChanceMagnitude = 0.f; // Percentage
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChanceMagnitude);
	TargetBlockChanceMagnitude = FMath::Clamp(TargetBlockChanceMagnitude, 0.f, 100.f);
	
	// If Block, halve the damage
	const bool bBlocked = FMath::RandRange(1.f, 100.f) < TargetBlockChanceMagnitude;
	Damage = bBlocked ? Damage / 2 : Damage;
	
	//~ Armor & Armor Penetration
	
	// Capture Armor on Target - Reduces a % of the Damage taken
	float TargetArmorMagnitude = 0.f; // Percentage
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmorMagnitude);
	TargetArmorMagnitude = FMath::Max(TargetArmorMagnitude, 0.f);
	
	// Capture ArmorPenetration on Source - Ignores % of Target's Armor
	float SourceArmorPenetrationMagnitude = 0.f; // Percentage
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetrationMagnitude);
	SourceArmorPenetrationMagnitude = FMath::Clamp(SourceArmorPenetrationMagnitude, 0.f, 400.f);
	
	// Armor calculation (Armor * (1 - Armor Penetration)) [Armor Penetration scales by 25% - i.e. needs 4 point to lower 1% of Armor]
	const float EffectiveArmor = TargetArmorMagnitude *= (100.f - SourceArmorPenetrationMagnitude * 0.25f) / 100.f;
	
	// Apply Armor to Damage (3 point of Effect Armor lowers 1% of Damage)
	Damage *= (100.f - EffectiveArmor * 0.333f) / 100.f;
	
	// Build the Execution Output
	// Add any output modifier that need change
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
