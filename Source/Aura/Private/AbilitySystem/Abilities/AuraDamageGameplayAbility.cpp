


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
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data, TargetASC);
	}
}

FAuraDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
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
	Params.KnockbackChance = KnockbackChance;
	Params.KnockbackMagnitude = KnockbackMagnitude;
	Params.DamageTypes = DamageTypes;
	// Check for Radial damage
	// For now, Radial damage is applied to all types in the Ability
	if (bIsRadialDamage)
	{
		Params.bIsRadialDamage = true;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
		Params.RadialDamageOrigin = RadialDamageOrigin;
	}

	// If the target actor is nullptr, then return early as there is nothing more to do
	if (!TargetActor)
	{
		return Params;
	}
	
	// Otherwise, make Knockback and Death Impulse
	if (bIsRadialDamage)
	{
		// If we are using a radial damage then make the impulse from its origin point
		Params.MakeDeathImpulseAndKnockback(RadialDamageOrigin, TargetActor->GetActorLocation());

	}
	else if (AActor* SourceActor = GetAvatarActorFromActorInfo())
	{
		// If there is an avatar actor for these params then use it as the source of the impulse
		Params.MakeDeathImpulseAndKnockback(SourceActor->GetActorLocation(), TargetActor->GetActorLocation());
	}

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
