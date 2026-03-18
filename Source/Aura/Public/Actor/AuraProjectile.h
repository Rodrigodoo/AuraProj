

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;
/*
 * Aura projectile actor used by the main character to hit enemies
 */
UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraProjectile();
	
	// Projectile component to guide the actor
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	// A damage effect that goes with the projectile
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	
private:

	// Sphere component for projectile collisions
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	
	// The life span of this actor (seconds)
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;
	
	//~ Begin - Effects
	// Sound object while projectile is flying
	// Note: should be a looping sound
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> FlyingSound;
	
	// Audio component to track any flying sound spawned
	UPROPERTY()
	TObjectPtr<UAudioComponent> FlyingAudioComponent;
	
	// Niagara system for the projectile impact
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	// Sound object for impact sound
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;
	//~ End - Effects
	
	// If the actor overlapped a target
	// Note: Only use on client!
	bool bHit = false;

	// To be called when the sphere is overlapped
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
