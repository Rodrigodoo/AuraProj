

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AuraLoadMenuSaveGame.generated.h"

/**
 * Save Game Object used in the Load Menu
 */
UCLASS()
class AURA_API UAuraLoadMenuSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	//~ Start - Id Variables

	// Slot Index for this Save Game Slot
	UPROPERTY()
	int32 SlotIndex = 0;
	//~ End - Id Variables
	
	//~ Start - Recorded Variables
	
	// Player Name
	FString PlayerName = FString("Default Name");
	//~ End - Recorded Variables
};
