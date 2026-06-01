


#include "AbilitySystem/Abilities/AuraGameplayAbilityBase.h"

FString UAuraGameplayAbilityBase::GetDescription(const int32 Level) const
{
	return FString::Printf(TEXT("%s, Level:<Level>%d</>"), L"Default Ability Name - LoremIpsum", Level);
}

FString UAuraGameplayAbilityBase::GetNextLevelDescription(const int32 NextLevel) const
{
	return FString::Printf(TEXT("Next Level:<Level>%d</> \nCauses much more damage"), NextLevel);
}

FString UAuraGameplayAbilityBase::GetLockedDescription(const int32 Level)
{
	return FString::Printf(TEXT("Spell Locked until level:<Level>%d</>"), Level);

}
