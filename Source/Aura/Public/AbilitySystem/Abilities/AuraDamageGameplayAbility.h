

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbilityBase.h"
#include "AuraDamageGameplayAbility.generated.h"

// Debuff struct to gather all the debuffs cause by a certain damage type
USTRUCT(BlueprintType)
struct FAuraDebuff
{
	GENERATED_BODY()
	
	// Percentage chance to cause the debuff
	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	float DebuffChange = 20.f;
	
	// Damage cause by the debuff
	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	float DebuffDamage = 5.f;
	
	// Frequency that the debuff triggers
	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	float DebuffFrequency = 1.f;
	
	// Duration of the Debuff
	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	float DebuffDuration = 5.f;
};

// Damage struct that holds the damage caused by this ability with a specific type and the debuffs incurred
USTRUCT(BlueprintType)
struct FAuraDamage
{
	GENERATED_BODY()
	
	// Damage for this type
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage = 1.f;
	
	// Debuff for this type
	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	FAuraDebuff Debuff = FAuraDebuff();
	
	// Return a scaled value for the respective level
	float GetValueAtLevel(float Level, const FString* ContextString = nullptr) const;
};

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
	float GetDamageByTypeAndLevel(const FGameplayTag& DamageType, const float Level) const;
	
	// Get the total damage value (all damage type) for said level
	float GetTotalDamageByLevel(const float Level) const;
	
protected:
	// Damage Effect class to add to the projectile
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	// All existing damage types for this ability
	// Map between gameplay tags and scalable floats
	UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (Categories = "Damage"))
	TMap<FGameplayTag, FAuraDamage> DamageTypes;
};
