


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	// Early check
	checkf(NumMinionsToSpawn > 0, TEXT("Invalid number of minions to spawn! Must be greater than 1."));
	
	// Get the forward vector and the location of the caster
	const FVector CasterForwardVector = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector CasterLocation = GetAvatarActorFromActorInfo()->GetActorLocation();

	// Location to Spawn around the forward vector
	TArray<FVector> SpawnLocations;
	
	// Lambda to store the spawn locations
	auto AddSpawnLocation = [&](const FVector& SpawnDirection)
	{
		FVector SpawnLocation = UAuraAbilitySystemLibrary::GetRandomLocationInLine(
			CasterLocation, SpawnDirection, MinSpawnDistance, MaxSpawnDistance);
		SetValidSpawnLocation(SpawnLocation);
		SpawnLocations.Add(SpawnLocation);
	};
	
	// If Odd (Spawn first in the forward vector then around it until the spread limit)
	if (NumMinionsToSpawn % 2)
	{
		// Spawn in random location in the Forward Vector
		AddSpawnLocation(CasterForwardVector);
		
		// Spawn the remaining minions (using safe divide as there might only be one minion)
		const float DeltaSpread = UKismetMathLibrary::SafeDivide(SpawnSpread, (NumMinionsToSpawn - 1));
		const float NumMinionsToSpawnOnEachSide = (NumMinionsToSpawn - 1) / 2;
		for (int32 i = 0; i < NumMinionsToSpawnOnEachSide; i++)
		{
			// Angle from Forward vector to rotate the spawn direction
			const float DeltaAngle = DeltaSpread + DeltaSpread * i;
			
			// Spawn in a random location in the Right Direction
			AddSpawnLocation(CasterForwardVector.RotateAngleAxis(DeltaAngle, FVector::UpVector));
			
			// Spawn in a random location in the Left Direction
			AddSpawnLocation(CasterForwardVector.RotateAngleAxis(-DeltaAngle, FVector::UpVector));
		}
	}
	else // If even (Spawn around the forward vector it until the spread limit - will not spawn on the forward vector)
	{
		// Spawn the minions
		const float DeltaSpread = SpawnSpread / NumMinionsToSpawn;
		const float NumMinionsToSpawnOnEachSide = NumMinionsToSpawn / 2;
		for (int32 i = 0; i < NumMinionsToSpawnOnEachSide; i++)
		{
			// Angle from Forward vector to rotate the spawn direction
			const float DeltaAngle = DeltaSpread + DeltaSpread * i;
			
			// Spawn in a random location in the Right Direction
			AddSpawnLocation(CasterForwardVector.RotateAngleAxis(DeltaAngle, FVector::UpVector));
			
			// Spawn in a random location in the Left Direction
			AddSpawnLocation(CasterForwardVector.RotateAngleAxis(-DeltaAngle, FVector::UpVector));
		}
	}
	
	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	if (MinionClassesToSpawn.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("No minion classes found!"));
		return nullptr;
	}
	
	return MinionClassesToSpawn[FMath::RandRange(0, MinionClassesToSpawn.Num() - 1)];
}

void UAuraSummonAbility::SetValidSpawnLocation(FVector& SpawnLocation) const
{
	// Do a line trace from sky to below the spawn location
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, 
		SpawnLocation + FVector::UpVector * 400.f, 
		SpawnLocation - FVector::UpVector * 400.f, 
		ECC_Visibility);
	
	if (HitResult.bBlockingHit)
	{
		SpawnLocation = HitResult.ImpactPoint;
	}
}
