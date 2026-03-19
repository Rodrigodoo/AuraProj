

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class UAuraCharacterClassInfoDataAsset;
/**
 * Game mode for Aura Project.
 * Stores any data for the game as a whole and any rules the game has
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// Data Asset that stores all the information relating to Enemies' RPG Class
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UAuraCharacterClassInfoDataAsset> CharacterClassInfoDataAsset;
};
