


#include "Actor/AuraFireBall.h"

#include "GameFramework/ProjectileMovementComponent.h"

AAuraFireBall::AAuraFireBall()
{
	ProjectileMovementComponent->PrimaryComponentTick.bCanEverTick = false;
	ProjectileMovementComponent->SetAutoActivate(false);
}

void AAuraFireBall::BeginPlay()
{
	// Call the AuraProjectile code
	Super::BeginPlay();
	// Start the moving logic
	StartOutgoingTimeline();
}

void AAuraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
