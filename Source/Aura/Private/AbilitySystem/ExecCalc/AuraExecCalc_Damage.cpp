


#include "AbilitySystem/ExecCalc/AuraExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTagsManager.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraCharacterClassInfoDataAsset.h"
#include "Interaction/AuraCombatInterface.h"

// Internal struct to store Attribute Capture Definitions
struct AuraDamageStatics
{
	// Attribute Capture Declarations 
	// Secondary Attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	// Resistance Attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	
	// Map from Resistance Gameplay Tags to Attribute definitions
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefinitions;
	
	AuraDamageStatics()
	{
		// Attribute Capture Definitions 
		// Secondary Attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
		// Resistance Attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);
		
		// Map Resistance tags to capture definitions
		TagsToCaptureDefinitions.Add(AuraGameplayTagsManager::Attributes_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDefinitions.Add(AuraGameplayTagsManager::Attributes_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureDefinitions.Add(AuraGameplayTagsManager::Attributes_Resistance_Arcane, ArcaneResistanceDef);
		TagsToCaptureDefinitions.Add(AuraGameplayTagsManager::Attributes_Resistance_Physical, PhysicalResistanceDef);
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
	// Secondary Attributes
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	// Resistance Attributes
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UAuraExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                  FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	
	const AActor* SourceAvatar = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	//~  Begin - Early checks
	//
	// Reasons for failure:
	// - No valid Source or Target Ability System Component
	// - No valid Source or Target Avatar Actors
	// - Source or Target Avatar Actors are not using a Combat Interface
	// - Called from Client
	// - No Character Class Info DataAsset -> No Damage Calculation Coefficients Curve Table -> No Curves set
	check(SourceAvatar)
	check(TargetAvatar)
	check(SourceAvatar->Implements<UAuraCombatInterface>());
	check(TargetAvatar->Implements<UAuraCombatInterface>());
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
	
	// Get the Effect Context Handle to pass information into the Effect
	FGameplayEffectContextHandle EffectContextHandle = EffectSpec.GetContext();
	
	/*
	 * Debuff Calculations
	 */
	DetermineDebuffs(ExecutionParams, EffectSpec, EvaluationParameters);
	
	/*
	 * Damage Calculation (By Order):
	 * - Block Chance
	 * - Effective Armor (Armor + Armor Penetration)
	 * - Critical Hit (Critical Hit Chance + Critical Hit Damage)
	 */
	
	// Get Damage Set by Caller Magnitude (Damage coming from Source)
	// Incoming Damage is the sum of all incoming damage types
	// Key: Damage Tag (FGameplayTag) | Value: Resistance Tag (FGameplayTag)
	float Damage = 0.f;
	for (const auto& Pair : AuraGameplayTagsManager::DamageTypesToResistances)
	{
		// Early check
		checkf(DamageStatics().TagsToCaptureDefinitions.Contains(Pair.Value), 
			TEXT("TagsToCaptureDefinitions does not contain tag: [%s]"), *Pair.Value.ToString());
		
		// Damage type value
		// If value not found it returns 0.f.
		float DamageTypeValue = EffectSpec.GetSetByCallerMagnitude(Pair.Key, false);
		
		// Damage type resistance definition
		const FGameplayEffectAttributeCaptureDefinition ResistanceDefinition = DamageStatics().TagsToCaptureDefinitions[Pair.Value];

		// Capture Resistance to a specific damage type on Target
		float TargetResistanceMagnitude = 0.f; // Percentage
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ResistanceDefinition, EvaluationParameters, TargetResistanceMagnitude);
		TargetResistanceMagnitude = FMath::Clamp(TargetResistanceMagnitude, 0.f, 100.f);
		
		// Reduce the damage type by the resistance percentage
		// DamageTypeValue = DamageTypeValue * (100 - Resistance Percentage)
		DamageTypeValue *= (100.f - TargetResistanceMagnitude) / 100.f;
		
		// Apply the type damage to the current Incoming Damage
		Damage += DamageTypeValue;
	}

	//~ Block Chance
	
	// Capture Block Chance on Target, and determine if there was a successful Block
	float TargetBlockChanceMagnitude = 0.f; // Percentage
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChanceMagnitude);
	TargetBlockChanceMagnitude = FMath::Clamp(TargetBlockChanceMagnitude, 0.f, 100.f);
	
	// If Block, halve the damage
	const bool bBlocked = FMath::RandRange(1.f, 100.f) < TargetBlockChanceMagnitude;
	Damage = bBlocked ? Damage / 2.f : Damage;
	
	// Signal the Gameplay Effect Context if it was a Blocked Hit
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	
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
	const int32 SourceCharacterLevel = IAuraCombatInterface::Execute_GetCharacterLevel(SourceAvatar);
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCharacterLevel);
	
	// Effective Armor calculation - (Armor * (1 - (Armor Penetration * Coefficient)))
	const float EffectiveArmor = TargetArmorMagnitude * 
		(100.f - SourceArmorPenetrationMagnitude * ArmorPenetrationCoefficient) / 100.f;
	
	// Capture Effective Armor coefficient
	const int32 TargetCharacterLevel = IAuraCombatInterface::Execute_GetCharacterLevel(TargetAvatar);
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCharacterLevel);
	
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
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCharacterLevel);
	
	// Capture Critical Hit Damage on Source
	float SourceCriticalHitDamageMagnitude = 0.f; // Damage Value
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamageMagnitude);
	SourceCriticalHitDamageMagnitude = FMath::Max(SourceCriticalHitDamageMagnitude, 0.f);
	
	// Calculate the Effective Critical Hit Chance - (Critical Hit Chance - (Critical Hit Resistance * Coefficient))
	const float EffectiveCriticalHitChance = SourceCriticalHitChanceMagnitude - (TargetCriticalHitResistanceMagnitude * CriticalHitResistanceCoefficient);
	
	// If it was a Critical Hit - Double damage and add Critical Hit Damage bonus
	const bool bCriticalHit = FMath::RandRange(1.f, 100.f) < EffectiveCriticalHitChance;
	Damage = bCriticalHit ? (Damage * 2.f) + SourceCriticalHitDamageMagnitude : Damage;
	
	// Signal the Gameplay Effect Context if it was a Critical Hit
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
	// Build the Execution Output
	// Add any output modifier that need change
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

void UAuraExecCalc_Damage::DetermineDebuffs(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& EffectSpec, FAggregatorEvaluateParameters EvaluationParameters) const
{
	// Loop over all debuffs and apply them
	// Key: Damage type Tag (FGameplayTag) | Value: Debuff Tag (FGameplayTag)
	for (const auto& Pair : AuraGameplayTagsManager::DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		
		// Damage type value
		// If value not found it returns -1.f.
		float DamageTypeValue = EffectSpec.GetSetByCallerMagnitude(DamageType, false, -1.f);
		if (DamageTypeValue < 0.f)
		{
			continue;
		}
		
		// Determine if there was a successful debuff
		const float SourceDebuffChance = EffectSpec.GetSetByCallerMagnitude(AuraGameplayTagsManager::GetDebuffChanceByType(DebuffType), 
		                                                              false, -1.f);
		if (SourceDebuffChance < 0.f)
		{
			continue;
		}
		
		// Target's Resistance to this debuff type 
		float TargetDebuffResistance = 0.f; // Percentage
		const FGameplayTag& ResistanceTag = AuraGameplayTagsManager::DamageTypesToResistances[DamageType];
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TagsToCaptureDefinitions[ResistanceTag], 
		                                                           EvaluationParameters, TargetDebuffResistance);
		TargetDebuffResistance = FMath::Clamp(TargetDebuffResistance, 0.f, 100.f);
		
		// The Chance this debuff actually applies
		// The target's resistances removes percentage points from the source debuff chance to apply
		// Effective Debuff Chance = SourceDebuffChance * (100 - TargetDebuffResistance) / 100.f
		const float EffectiveDebuffChance = SourceDebuffChance * (100 -TargetDebuffResistance) / 100.f;
		if (EffectiveDebuffChance <= 0.f)
		{
			// No chance to apply
			continue;
		}
		
		// Check if debuff is applied
		// Use random number generator and if the number is was the same or above, the debuff was not applied
		if (FMath::RandRange(1, 100) >= EffectiveDebuffChance)
		{
			continue;
		}
		
		// TODO: Apply Debuff
	}
}
