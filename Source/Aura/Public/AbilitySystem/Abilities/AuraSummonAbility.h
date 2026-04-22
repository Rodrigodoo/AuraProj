

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbilityBase.h"
#include "AuraSummonAbility.generated.h"

/**
 * Ability to summon minions into game
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	// Get the available spawn location for the minions (centered around the caster's forward vector)
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();
	
	// Number of minions to spawn
	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", UIMin = "1"), Category = "Summoning")
	int32 NumMinionsToSpawn = 5;
	
	// The classes of the minions to spawn
	UPROPERTY(EditAnywhere, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClassesToSpawn;
	
	// Minimum distance from the caster to spawn
	UPROPERTY(EditAnywhere, Category = "Summoning")
	float MinSpawnDistance = 50.0f;
	
	// Maximum distance from the caster to spawn
	UPROPERTY(EditAnywhere, Category = "Summoning")
	float MaxSpawnDistance = 250.0f;
	
	// Degree of spread for spawning minions (minions will be spawn in front of the caster in area limited by this spread)
	UPROPERTY(EditAnywhere, Category = "Summoning")
	float SpawnSpread = 90.f;
};
