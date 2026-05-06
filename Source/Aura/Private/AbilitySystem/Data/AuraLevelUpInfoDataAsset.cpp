


#include "AbilitySystem/Data/AuraLevelUpInfoDataAsset.h"

#include "WorldPartition/Cook/WorldPartitionCookPackage.h"

int32 UAuraLevelUpInfoDataAsset::FindLevelForXP(const int32 XP) const
{
	// Loop through all the levels (They need to be ordered by Level!)
	// If the XP provided is bellow the XP requirement then the character is at that Level
	int32 Level = 1;
	for (const FLevelUpInfo& LevelUpInfo : LevelUpInfoData)
	{
		if (XP <= LevelUpInfo.XPRequirement)
		{
			Level = LevelUpInfo.Level;
			break;
		}
	}
	
	return Level;
}

void UAuraLevelUpInfoDataAsset::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Early exit
	if (!PropertyChangedEvent.Property)
	{
		return;
	}
	
	// If changing the Level up Array then make sure the Levels of each struct are correct
	if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UAuraLevelUpInfoDataAsset, LevelUpInfoData))
	{
		UpdateLevelValuesInLevelUpInfoData();
	}
}

void UAuraLevelUpInfoDataAsset::UpdateLevelValuesInLevelUpInfoData()
{
	// Make sure the levels are set up to be equal to a 1 based index [Level = index + 1]
	for (int i = 0; i < LevelUpInfoData.Num(); ++i)
	{
		LevelUpInfoData[i].Level = i + 1;
	}
}
