

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
/**
 * Spell to spawn projectiles
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	// Retrieves the projectile count that can be spawned at once
	int32 GetProjectileCount(int32 Level) const;
protected:
	//~ Begin - UGameplayAbility overrides
	
	// Define Ability behaviour
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//~ End - UGameplayAbility overrides
	
	// Spawns the Fire Bolt projectile.
	// This method should be called after the Gameplay Event triggers it in Blueprint
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = 3), Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& MontageTag, 
		bool bOverridePitch = false, float PitchOverride = 0.f);
	
	// Projectile class to spawn when ability is activated
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;
	
	// Amount of projectiles that are being spawned at once
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FScalableFloat ProjectileCount = 1;
};
