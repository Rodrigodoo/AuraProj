

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
	// Retrieves the Attribute points reward for said Level
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(const int32 Level) const;
	
	// Retrieves the Spell points reward for said Level
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(const int32 Level) const;
	
	// Retrieves the Level for the provided XP
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(const int32 XP) const;
	
	// Gets the player's XP
	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerXP() const;
	
	// Adds XP to the Player
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerXP(const int32 XPToAdd);
	
	// Gets the player's currently available attribute points
	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerAttributePoints() const;
	
	// Adds points the player's currently available attribute points
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerAttributePoints(const int32 PointsToAdd) const;
	
	// Gets the player's currently available spell points
	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerSpellPoints() const;
	
	// Adds points the player's currently available spell points
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerSpellPoints(const int32 PointsToAdd) const;
	
	// Levels up the player
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
};
