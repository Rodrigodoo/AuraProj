


#include "AbilitySystem/Abilities/AuraFireBolt.h"

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
	FRotator SpawnRotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		// If the pitch override flag is set, then override the pitch
		SpawnRotation.Pitch = PitchOverride;
	}

	const int32 NumProjectiles = ProjectileCount.GetValueAtLevel(GetAbilityLevel());
	if (NumProjectiles <= 1)
	{
		// Spawn only one projectile
		SpawnProjectile(ProjectileTargetLocation, MontageTag, bOverridePitch, PitchOverride);
		return;
	}
	
	// Now spawn the projectiles inside of the projectile spread
	const FVector ForwardVector = SpawnRotation.Vector();
	FVector DebugVector;
	// If Odd (Spawn first bolt in the forward vector then around it until the spread limit)
	if (NumProjectiles % 2)
	{
		// Spawn in the Forward Vector
		DebugVector = ForwardVector;
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation,
										 SocketLocation + DebugVector * 100.f, 5, FLinearColor::Blue, 120.f,
										 2);
		
		// Spawn the remaining bolts (should be a safe divide since we checked for NumProjectiles <= 1 before)
		const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);
		const float NumMProjectilesToSpawnOnEachSide = (NumProjectiles - 1) / 2;
		for (int32 i = 0; i < NumMProjectilesToSpawnOnEachSide; i++)
		{
			// Angle from Forward vector to rotate the spawn direction
			const float DeltaAngle = DeltaSpread + DeltaSpread * i;
			
			// Spawn in a random location in the Right Direction (DeltaAngle)
			DebugVector = ForwardVector.RotateAngleAxis(DeltaAngle, FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation,
										 SocketLocation + DebugVector * 100.f, 5, FLinearColor::Red, 120.f,
										 2);

			
			// Spawn in a random location in the Left Direction (-DeltaAngle)
			DebugVector = ForwardVector.RotateAngleAxis(-DeltaAngle, FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation,
										 SocketLocation + DebugVector * 100.f, 5, FLinearColor::Green, 120.f,
										 2);

		}
	}
	else // If even (Spawn around the forward vector it until the spread limit - will not spawn on the forward vector)
	{
		// Spawn the bolts
		const float DeltaSpread = ProjectileSpread / NumProjectiles;
		const float NumMProjectilesToSpawnOnEachSide = NumProjectiles / 2;
		for (int32 i = 0; i < NumMProjectilesToSpawnOnEachSide; i++)
		{
			// Angle from Forward vector to rotate the spawn direction
			const float DeltaAngle = DeltaSpread + DeltaSpread * i;
			
			// Spawn in a random location in the Right Direction (DeltaAngle)
			DebugVector = ForwardVector.RotateAngleAxis(DeltaAngle, FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation,
							 SocketLocation + DebugVector * 100.f, 5, FLinearColor::Red, 120.f,
							 2);
			
			// Spawn in a random location in the Left Direction (-DeltaAngle)
			DebugVector = ForwardVector.RotateAngleAxis(-DeltaAngle, FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation,
							 SocketLocation + DebugVector * 100.f, 5, FLinearColor::Green, 120.f,
							 2);

		}
	}
	

	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation,
	                                     SocketLocation + SpawnRotation.Vector() * 100.f, 5, FLinearColor::White, 120.f,
	                                     2);
}
