

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * Ability that fires Fire bolt shots (damage projectile)
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
	
public:
	// Spawns all the fire bolt projectiles for this ability level
	// This method should be called after the Gameplay Event triggers it in Blueprint
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = 4), Category = "Projectile")
	virtual void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& MontageTag, 
	                              AActor* HomingTargetActor = nullptr, const bool bOverridePitch = false, const float PitchOverride = 0.f) override;
	
protected:
	// Minimum acceleration of the homing projectile
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMin = 1600.f;
	
	// Minimum acceleration of the homing projectile
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMax = 3200.f;
	
	// Should projectiles home in on the target
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	bool bLaunchHomingProjectiles = true;
};
