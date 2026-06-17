

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/Abilities/AuraGameplayAbilityBase.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * Base Ability specialized on dealing damage
 * Note: Inherit from this Ability if your Ability inflicts Damage
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	// Causes damage to the TargetActor
	// Goes through all available damage types to set by caller magnitude
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
	// Creates Damage Effect Params from this classes default settings
	// Can provide a target actor if it is known.
	UFUNCTION(BlueprintPure)
	FAuraDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;
	
	// Get the damage value for said type and level
	float GetDamageByTypeAndLevel(const FGameplayTag& DamageType, const float Level) const;
	
	// Get the total damage value (all damage type) for said level
	float GetTotalDamageByLevel(const float Level) const;
	
protected:
	// Damage Effect class to add to the projectile
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	// Whenever this ability causes death, it applies this impulse to the target
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|DeathImpulse")
	float DeathImpulseMagnitude = 10000.f;
	
	// chance this ability causes knockback, it applies this impulse to the target
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Knockback")
	float KnockbackChance = 20.f;
	
	// Whenever this ability causes knockback, it applies this impulse to the target
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Knockback")
	float KnockbackMagnitude = 500.f;
	
	// The pitch override for the knockback (degrees)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Knockback")
	float KnockbackPitchOverride = 45.0f;
	
	// All existing damage types for this ability
	// Map between gameplay tags and scalable floats
	UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (Categories = "Damage"))
	TMap<FGameplayTag, FAuraDamage> DamageTypes;
};
