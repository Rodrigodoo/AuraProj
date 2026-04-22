


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	// Early check
	checkf(NumMinionsToSpawn > 0, TEXT("Invalid number of minions to spawn! Must be greater than 1."));
	
	// Get the forward vector and the location of the caster
	const FVector CasterForwardVector = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector CasterLocation = GetAvatarActorFromActorInfo()->GetActorLocation();

	// Location to Spawn around the forward vector
	TArray<FVector> SpawnLocations;
	
	// If Odd (Spawn first in the forward vector then around it until the spread limit)
	if (NumMinionsToSpawn % 2)
	{
		// Spawn in random location Forward Vector
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
		CasterLocation, CasterLocation + CasterForwardVector * MaxSpawnDistance, 4.f, FColor::Blue, 3.f);
		FVector SpawnLocation = UAuraAbilitySystemLibrary::GetRandomLocationInLine(
			CasterLocation, CasterForwardVector, MinSpawnDistance, MaxSpawnDistance);
		DrawDebugSphere(GetWorld(), SpawnLocation, 15.f, 12, FColor::Cyan, false, 3.f);
		SpawnLocations.Add(SpawnLocation);
		
		// Spawn the remaining minions (using safe divide as there might only be one minion)
		const float DeltaSpread = UKismetMathLibrary::SafeDivide(SpawnSpread, (NumMinionsToSpawn - 1));
		const float NumMinionsToSpawnOnEachSide = (NumMinionsToSpawn - 1) / 2;
		for (int32 i = 0; i < NumMinionsToSpawnOnEachSide; i++)
		{
			// Angle from Forward vector to rotate the spawn direction
			const float DeltaAngle = DeltaSpread + DeltaSpread * i;
			
			// Spawn in a random location in the Right Direction
			const FVector SpawnDirectionRight = CasterForwardVector.RotateAngleAxis(DeltaAngle, FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
			CasterLocation, CasterLocation + SpawnDirectionRight * MaxSpawnDistance, 4.f, FColor::Blue, 3.f);
			SpawnLocation = UAuraAbilitySystemLibrary::GetRandomLocationInLine(
			CasterLocation, SpawnDirectionRight, MinSpawnDistance, MaxSpawnDistance);
			DrawDebugSphere(GetWorld(), SpawnLocation, 15.f, 12, FColor::Cyan, false, 3.f);
			SpawnLocations.Add(SpawnLocation);
			
			// Spawn in a random location in the Left Direction
			const FVector SpawnDirectionLeft = CasterForwardVector.RotateAngleAxis(-DeltaAngle, FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
			CasterLocation, CasterLocation + SpawnDirectionLeft * MaxSpawnDistance, 4.f, FColor::Blue, 3.f);
			SpawnLocation = UAuraAbilitySystemLibrary::GetRandomLocationInLine(
			CasterLocation, SpawnDirectionLeft, MinSpawnDistance, MaxSpawnDistance);
			DrawDebugSphere(GetWorld(), SpawnLocation, 15.f, 12, FColor::Cyan, false, 3.f);
			SpawnLocations.Add(SpawnLocation);
		}
	}
	else // If even (Spawn around the forward vector it until the spread limit - will not spawn on the forward vector)
	{
		// Spawn the minions
		const float DeltaSpread = SpawnSpread / NumMinionsToSpawn;
		const float NumMinionsToSpawnOnEachSide = NumMinionsToSpawn / 2;
		for (int32 i = 0; i < NumMinionsToSpawnOnEachSide; i++)
		{
			FVector SpawnLocation;

			// Angle from Forward vector to rotate the spawn direction
			const float DeltaAngle = DeltaSpread + DeltaSpread * i;
			const FVector SpawnDirectionRight = CasterForwardVector.RotateAngleAxis(DeltaAngle, FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
			CasterLocation, CasterLocation + SpawnDirectionRight * MaxSpawnDistance, 4.f, FColor::Blue, 3.f);
			SpawnLocation = UAuraAbilitySystemLibrary::GetRandomLocationInLine(
			CasterLocation, SpawnDirectionRight, MinSpawnDistance, MaxSpawnDistance);
			DrawDebugSphere(GetWorld(), SpawnLocation, 15.f, 12, FColor::Cyan, false, 3.f);
			SpawnLocations.Add(SpawnLocation);
			
			const FVector SpawnDirectionLeft = CasterForwardVector.RotateAngleAxis(-DeltaAngle, FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
			CasterLocation, CasterLocation + SpawnDirectionLeft * MaxSpawnDistance, 4.f, FColor::Blue, 3.f);
			SpawnLocation = UAuraAbilitySystemLibrary::GetRandomLocationInLine(
			CasterLocation, SpawnDirectionLeft, MinSpawnDistance, MaxSpawnDistance);
			DrawDebugSphere(GetWorld(), SpawnLocation, 15.f, 12, FColor::Cyan, false, 3.f);
			SpawnLocations.Add(SpawnLocation);
		}
	}
	
	return SpawnLocations;
	
}
