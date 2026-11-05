


#include "AbilitySystem/Abilities/AuraFireBlast.h"

int32 UAuraFireBlast::GetFireBallsCount(const int32 AbilityLevel) const
{
	return FireBalls.GetValueAtLevel(AbilityLevel);
}

int32 UAuraFireBlast::GetCurrentFireBallsCount() const
{
	return GetFireBallsCount(GetAbilityLevel());
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	return TArray<AAuraFireBall*>();
}
