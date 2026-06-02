

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbilityBase.generated.h"

/**
 * Aura's base Gameplay Ability class. Other Gameplay Abilities for this project should inherit from it.
 */
UCLASS()
class AURA_API UAuraGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	// Input Gameplay Tag for the startup ability
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (Categories = "InputTag"))
	FGameplayTag StartupInputTag;
	
	//~ Begin - Ability Getters
	// Retrieves the cost to activate this ability
	virtual float GetManaCost(int32 Level) const;
	
	// Retrieves the cooldown to activate this ability
	virtual float GetCooldown(int32 Level) const;
	//~ End - Ability Getters
};
