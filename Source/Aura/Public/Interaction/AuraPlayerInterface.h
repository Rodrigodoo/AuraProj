

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AuraPlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAuraPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface to be used by player classes
 */
class AURA_API IAuraPlayerInterface
{
	GENERATED_BODY()

public:
	// Adds XP to the Player
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerXP(const int32 XPToAdd);
	
	// Levels up the player
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
};
