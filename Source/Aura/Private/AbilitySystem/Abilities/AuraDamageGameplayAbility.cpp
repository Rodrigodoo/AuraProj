


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
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	;
}

float UAuraDamageGameplayAbility::GetDamageAtLevel(const FGameplayTag& DamageType, const float Level) const
{
	const FScalableFloat* Damage = DamageTypes.Find(DamageType);
	if (!Damage)
	{
		return 0.0f;
	}
	
	return Damage->GetValueAtLevel(Level);
}
