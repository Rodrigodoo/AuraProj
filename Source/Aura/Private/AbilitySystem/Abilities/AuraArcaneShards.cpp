


#include "AbilitySystem/Abilities/AuraArcaneShards.h"

int32 UAuraArcaneShards::GetShardCount(const int32 AbilityLevel) const
{
	return ShardCount.GetValueAtLevel(AbilityLevel);
}

int32 UAuraArcaneShards::GetCurrentShardCount() const
{
	return GetShardCount(GetAbilityLevel());
}
