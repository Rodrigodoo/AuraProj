


#include "Actor/AuraFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
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
	// Check for valid overlaps
	if (!IsValidOverlap(OtherActor))
	{
		return;
	}
	
	// If we are the server
	if (HasAuthority())
	{
		// If the other actor as an ASC then apply the Gameplay Effect to it
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			// Get the death impulse and the knockback before applying the damage effect to the target
			DamageEffectParams.MakeDeathImpulseAndKnockback(GetActorLocation(), OtherActor->GetActorLocation());
			
			// Apply the effect to the target
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageGameplayEffect(DamageEffectParams);
		}
	}
}
