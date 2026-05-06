

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AuraLevelUpInfoDataAsset.generated.h"

/**
 * Data Asset to store information on main character's level up
 * - XP Requirements
 * - Attribute Points
 * - Spell points
 */

/*
 * Structure to store the Level Up information for said level
 */
USTRUCT(BlueprintType)
struct FLevelUpInfo
{
	GENERATED_BODY()

	// Current level (Should not be edited - based on array index)
	UPROPERTY(VisibleDefaultsOnly ,BlueprintReadOnly)
	int32 Level = 1;
	
	// XP Requirement to pass onto the next level
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 XPRequirement = 0;
	
	// Attribute points reward when reaching this level
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AttributePointReward = 1;
	
	// Spell points reward when reaching this level
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SpellPointReward = 1;
};

UCLASS()
class AURA_API UAuraLevelUpInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// Finds the level associated with the provided XP
	UFUNCTION(BlueprintCallable)
	int32 FindLevelForXP(const int32 XP) const;
	
	// All the level up data (needs to be in order!)
	// Note that the index of each entry is [Index = Level - 1]
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Level Up Data", meta=(TitleProperty = "Level"))
	TArray<FLevelUpInfo> LevelUpInfoData;
	
#if WITH_EDITOR
	// Make sure the FLevelUpInfo.Level member variable is updated according its position on the LevelUpInfoData array
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
private:
	// Update the Levels in the LevelUpInfoData as they should be [Level = index + 1]
	void UpdateLevelValuesInLevelUpInfoData();
};
