


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/AuraCombatInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& MontageTag,
                                     AActor* HomingTarget /*= nullptr*/, const bool bOverridePitch /*= false*/, const float PitchOverride /*= 0.f*/)
{
	// If we are not on the server then move along.
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}
	
	// If the Avatar implements a combat interface, 
	// then get the weapon's socket location to spawn the projectile
	if (!GetAvatarActorFromActorInfo()->Implements<UAuraCombatInterface>())
	{
		// Otherwise, for now we will not spawn the projectile
		return;
	}
	
	// Get the socket location from the interface (for the montage playing)
	const FVector SocketLocation = IAuraCombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(), MontageTag);
	
	// Get the rotation of the projectile to point at the target location
	// Vector from socket location to target location
	FRotator ForwardRotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		// If the pitch override flag is set, then override the pitch
		ForwardRotation.Pitch = PitchOverride;
	}

	const int32 NumProjectiles = ProjectileCount.GetValueAtLevel(GetAbilityLevel());
	if (NumProjectiles <= 1)
	{
		// Spawn only one projectile
		SpawnProjectile(ProjectileTargetLocation, MontageTag, bOverridePitch, PitchOverride);
		return;
	}
	
	// Now spawn the projectiles inside of the projectile spread
	const FVector ForwardVector = ForwardRotation.Vector();
	
	// Get the spawn Rotators evenly spread around the forward vector
	TArray<FRotator> SpawnRotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(ForwardVector, FVector::UpVector, ProjectileSpread, NumProjectiles);
	
	for (const FRotator& SpawnRotation : SpawnRotations)
	{
		// Spawn the projectile actor at the socket location and with the direction of the target
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(SpawnRotation.Quaternion());
	
		// Begin Spawning the projectile
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(), 
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
		// Setup Projectile
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		// Finish spawning
		Projectile->FinishSpawning(SpawnTransform);
	}
}
