


#include "AbilitySystem/Data/AuraLevelUpInfoDataAsset.h"

#include "Kismet/KismetMathLibrary.h"
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

float UAuraLevelUpInfoDataAsset::FindCurrentLevelPercent(const int32 XP) const
{
	const int32 CurrentLevel = FindLevelForXP(XP);
	const int32 LevelIndex = CurrentLevel - 1;
	if (LevelUpInfoData.Num() <= 0 || !LevelUpInfoData.IsValidIndex(LevelIndex))
	{
		// Early exit
		return 0.0f;
	}

	// If it is the first level, the minimum XP value will be 0
	if (CurrentLevel == 1)
	{
		return UKismetMathLibrary::SafeDivide(XP, LevelUpInfoData[LevelIndex].XPRequirement);
	}
	
	// Check if the previous level is valid (sanity check)
	if (!LevelUpInfoData.IsValidIndex(LevelIndex - 1))
	{
		return 0.0f;
	}
	
	// The total XP for this level will be:
	// the XP requirement for this level minus the XP requirement for the previous level
	const int32 TotalAmountOfXPForThisLevel = LevelUpInfoData[LevelIndex].XPRequirement - LevelUpInfoData[LevelIndex - 1].XPRequirement;
	const int32 CurrentXPForThisLevel = XP - LevelUpInfoData[LevelIndex - 1].XPRequirement;
	return UKismetMathLibrary::SafeDivide(CurrentXPForThisLevel, TotalAmountOfXPForThisLevel);
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
