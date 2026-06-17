


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	// Create a spec handle to store all damage type values and then apply them to the Target Actor
	//  Key: GameplayTag (Damage Type tag) | FScalableFloat (Damage value associated with type at a specific level)
	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	for (const auto& Pair: DamageTypes)
	{
		// Create a tag set by caller magnitude for this damage type
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
			DamageSpecHandle, Pair.Key, Pair.Value.GetValueAtLevel(GetAbilityLevel()));
	}

	// Apply  ability to the target actor
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data,
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FAuraDamageEffectParams UAuraDamageGameplayAbility::MakaDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	// Fill in with the default values
	FAuraDamageEffectParams Params;
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	// Note that GetAbilitySystemComponent can be null if TargetActor == null
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.TotalBaseDamage = GetTotalDamageByLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.DamageTypes = DamageTypes;
	
	return Params;
}

float UAuraDamageGameplayAbility::GetDamageByTypeAndLevel(const FGameplayTag& DamageType, const float Level) const
{
	const FAuraDamage* Damage = DamageTypes.Find(DamageType);
	if (!Damage)
	{
		return 0.0f;
	}
	
	return Damage->GetValueAtLevel(Level);
}

float UAuraDamageGameplayAbility::GetTotalDamageByLevel(const float Level) const
{
	float TotalDamage = 0.0f;
	for (const auto& Pair: DamageTypes)
	{
		// Get the damage from all types
		TotalDamage += Pair.Value.GetValueAtLevel(Level);
	}
	
	return TotalDamage;
}
