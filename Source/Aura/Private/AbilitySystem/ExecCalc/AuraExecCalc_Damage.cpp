


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
	 * Damage Calculation - Block Chance -> Effective Armor (Armor + Armor Penetration)
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
	
	// Build the Execution Output
	// Add any output modifier that need change
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
