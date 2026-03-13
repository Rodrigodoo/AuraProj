


#include "Interaction/AuraCombatInterface.h"

int32 IAuraCombatInterface::GetCharacterLevel()
{
	// By default, it will return 0 if not implemented
	return 0;
}

FVector IAuraCombatInterface::GetCombatSocketLocation() const
{
	// By default, it will return an empty vector if not implemented
	return FVector();
}
