

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbilityBase.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
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
	//~ End - UGameplayAbility overrides
	
	// Spawns the Fire Bolt projectile.
	// This method should be called after the Gameplay Event triggers it in Blueprint
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile();
	
	// Projectile class to spawn when ability is activated
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;
};
