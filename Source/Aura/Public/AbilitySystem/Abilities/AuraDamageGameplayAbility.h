

#pragma once

#include "CoreMinimal.h"
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
	
	// Get the damage value for said type and level
	float GetDamageAtLevel(const FGameplayTag& DamageType, const float Level) const;
	
protected:
	// Damage Effect class to add to the projectile
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// All existing damage types for this ability
	// Map between gameplay tags and scalable floats
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
