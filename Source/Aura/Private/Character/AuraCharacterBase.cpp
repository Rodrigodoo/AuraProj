


#include "Character/AuraCharacterBase.h"

AAuraCharacterBase::AAuraCharacterBase()
{
 	// No tick for this class
	PrimaryActorTick.bCanEverTick = false;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

