

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AuraGameInstance.generated.h"

class UAuraAbilityInfoDataAsset;
class UAuraCharacterClassInfoDataAsset;
/**
 * Game Instance for Aura Project.
 * Stores any data for the game as a whole
 */
UCLASS()
class AURA_API UAuraGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// Data Asset that stores all the information relating to Enemies' RPG Class
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UAuraCharacterClassInfoDataAsset> CharacterClassInfoDataAsset;
	
	// Data Asset that stores all the information relating to Abilities
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UAuraAbilityInfoDataAsset> AbilityInfoDataAsset;
};
