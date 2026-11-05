

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "AuraFireBall.generated.h"

/**
 * Projectile that goes a certain distance out and returns to the origin
 */
UCLASS()
class AURA_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()
public:
	AAuraFireBall();

protected:
	virtual void BeginPlay() override;
	
	// To be called when the Fire Ball is overlapped
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult) override;
	

};
