

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbilityBase.h"
#include "AuraProjectileSpell.generated.h"

/**
 * Spell to spawn projectiles
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraGameplayAbilityBase
{
	GENERATED_BODY()
	
protected:
	//~ Begin - UGameplayAbility overrides
	
	// Define Ability behaviour
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//~ Begin - UGameplayAbility overrides
};
