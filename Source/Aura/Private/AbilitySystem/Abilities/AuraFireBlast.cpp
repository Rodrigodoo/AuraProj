


#include "AbilitySystem/Abilities/AuraFireBlast.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireBall.h"
#include "Math/Rotator.h"

int32 UAuraFireBlast::GetFireBallsCount(const int32 AbilityLevel) const
{
	return NumFireBalls.GetValueAtLevel(AbilityLevel);
}

int32 UAuraFireBlast::GetCurrentFireBallsCount() const
{
	return GetFireBallsCount(GetAbilityLevel());
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	// If we are not on the server then move along.
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return TArray<AAuraFireBall*>();
	}
	
	const int32 FireBallCount = GetCurrentFireBallsCount();
	if (FireBallCount <= 0)
	{
		// Early check
		return TArray<AAuraFireBall*>();
	}
	// Spawned Fire Balls
	TArray<AAuraFireBall*> SpawnedFireBalls;
	
	// Get evenly spaced rotators around a forward vector
	const FVector ForwardVector = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(ForwardVector, FVector::UpVector, 360.f, FireBallCount);

	// Create the Spawn Transform
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetAvatarActorFromActorInfo()->GetActorLocation());

	// Spawn the Fire Balls evenly around the character
	for (const FRotator& Rotator : Rotators)
	{
		// Start Spawning the Fire Ball
		SpawnTransform.SetRotation(Rotator.Quaternion());
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass, 
			SpawnTransform, 
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		// Set the Effect Parameters (No Target yet)
		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults(nullptr);
		
		// Add Fire Ball to Array
		SpawnedFireBalls.Add(FireBall);
		
		// Finish Spawning the Fire Ball
		FireBall->FinishSpawning(SpawnTransform);
	}
	
	return SpawnedFireBalls;
}
