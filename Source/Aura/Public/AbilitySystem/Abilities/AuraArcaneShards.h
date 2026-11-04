

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraArcaneShards.generated.h"

/**
 * Arcane Shards Ability - Spawns shards in a point collection and cause damage radially from these points
 */
UCLASS()
class AURA_API UAuraArcaneShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	// Get the Shard Count for the ability level
	int32 GetShardCount(const int32 AbilityLevel) const;
	
	// Get the Shard Count for the current ability level
	UFUNCTION(BlueprintPure)
	int32 GetCurrentShardCount() const;

private:
	// Number of shards to be spawned
	UPROPERTY(EditDefaultsOnly, Category="Shards")
	FScalableFloat ShardCount;
};
