


#include "AbilitySystem/ExecCalc/AuraExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTagsManager.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraCharacterClassInfoDataAsset.h"
#include "Interaction/AuraCombatInterface.h"

// Internal struct to store Attribute Capture Definitions
struct AuraDamageStatics
{
	// Attribute Capture Declarations 
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	
	AuraDamageStatics()
	{
		// Attribute Capture Definitions 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
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
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UAuraExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	
	const AActor* SourceAvatar = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;
	const IAuraCombatInterface* SourceCombatInterface = Cast<IAuraCombatInterface>(SourceAvatar);
	const IAuraCombatInterface* TargetCombatInterface = Cast<IAuraCombatInterface>(TargetAvatar);

	//~  Begin - Early checks
	//
	// Reasons for failure:
	// - No valid Source or Target Ability System Component
	// - No valid Source or Target Avatar Actors
	// - Source or Target Avatar Actors are not using a Combat Interface
	// - Called from Client
	// - No Character Class Info DataAsset -> No Damage Calculation Coefficients Curve Table -> No Curves set
	check(SourceCombatInterface);
	check(TargetCombatInterface);
	UAuraCharacterClassInfoDataAsset* CharacterClassInfoDataAsset = 
		UAuraAbilitySystemLibrary::GetCharacterClassInfoDataAsset(SourceAvatar);
	if (!CharacterClassInfoDataAsset)
	{
		// Likely called from the client. Need to call from server!
		return;
	}
	const UCurveTable* DamageCalculationCoefficients = CharacterClassInfoDataAsset->DamageCalculationCoefficients;
	check(DamageCalculationCoefficients); // Need to set up the damage Calculation Coefficient Curve Table
	const FRealCurve* ArmorPenetrationCurve = DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	check(ArmorPenetrationCurve); // Need to set up ArmorPenetration Curve
	const FRealCurve* EffectiveArmorCurve = DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	check(EffectiveArmorCurve);// Need to set up EffectiveArmor Curve
	const FRealCurve* CriticalHitResistanceCurve = DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	check(CriticalHitResistanceCurve);// Need to set up CriticalHitResistance Curve
	//~  End - Early checks
	
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
	 * Damage Calculation (By Order):
	 * - Block Chance
	 * - Effective Armor (Armor + Armor Penetration)
	 * - Critical Hit (Critical Hit Chance + Critical Hit Damage)
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
	Damage = bBlocked ? Damage / 2.f : Damage;
	
	//~ Armor & Armor Penetration
	
	// Capture Armor on Target - Reduces a % of the Damage taken
	float TargetArmorMagnitude = 0.f; // Armor Value
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmorMagnitude);
	TargetArmorMagnitude = FMath::Max(TargetArmorMagnitude, 0.f);
	
	// Capture ArmorPenetration on Source - Ignores % of Target's Armor
	float SourceArmorPenetrationMagnitude = 0.f; // Percentage Point
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetrationMagnitude);
	SourceArmorPenetrationMagnitude = FMath::Max(SourceArmorPenetrationMagnitude, 0.f);
	
	// Capture Armor Penetration coefficient
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetCharacterLevel());
	
	// Effective Armor calculation - (Armor * (1 - (Armor Penetration * Coefficient)))
	const float EffectiveArmor = TargetArmorMagnitude * 
		(100.f - SourceArmorPenetrationMagnitude * ArmorPenetrationCoefficient) / 100.f;
	
	// Capture Effective Armor coefficient
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetCharacterLevel());
	
	// Apply Armor to Damage - (Damage * (1 - (Effective Armor * Coefficient)))
	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;
	
	//~ Critical Hit (Chance + Resistance + Damage)
	
	// Capture Critical Hit Chance on Source
	float SourceCriticalHitChanceMagnitude = 0.f; // Percentage
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChanceMagnitude);
	SourceCriticalHitChanceMagnitude = FMath::Clamp(SourceCriticalHitChanceMagnitude, 0.f, 100.f);
	
	// Capture Critical Hit Resistance on Target
	float TargetCriticalHitResistanceMagnitude = 0.f; // Percentage
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistanceMagnitude);
	TargetCriticalHitResistanceMagnitude = FMath::Clamp(TargetCriticalHitResistanceMagnitude, 0.f, 100.f);
	
	// Capture Critical Hit Resistance coefficient
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetCharacterLevel());
	
	// Capture Critical Hit Damage on Source
	float SourceCriticalHitDamageMagnitude = 0.f; // Damage Value
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamageMagnitude);
	SourceCriticalHitDamageMagnitude = FMath::Max(SourceCriticalHitDamageMagnitude, 0.f);
	
	// Calculate the Effective Critical Hit Chance - (Critical Hit Chance - (Critical Hit Resistance * Coefficient))
	const float EffectiveCriticalHitChance = SourceCriticalHitChanceMagnitude - (TargetCriticalHitResistanceMagnitude * CriticalHitResistanceCoefficient);
	
	// If it was a Critical Hit - Double damage and add Critical Hit Damage bonus
	const bool bCriticalHit = FMath::RandRange(1.f, 100.f) < EffectiveCriticalHitChance;
	Damage = bCriticalHit ? (Damage * 2.f) + SourceCriticalHitDamageMagnitude : Damage;
	
	// Build the Execution Output
	// Add any output modifier that need change
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
