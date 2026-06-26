

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
	
	// Spawns the ability's projectile.
	// This method should be called after the Gameplay Event triggers it in Blueprint
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = 3), Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& MontageTag, 
		const bool bOverridePitch = false, const float PitchOverride = 0.f);
	
	// Method to spawn multiple projectiles. Override in child classes.
	virtual void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& MontageTag, 
		AActor* HomingTarget = nullptr, const bool bOverridePitch = false, const float PitchOverride = 0.f);
	
	// Projectile class to spawn when ability is activated
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;
	
	// Amount of projectiles that are being spawned at once
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FScalableFloat ProjectileCount = 1;
	
	// Spread from where projectiles might be launched in case there is more than one
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ProjectileSpread = 90.f;
};
