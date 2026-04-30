


#include "AbilitySystem/Data/AuraAbilityInfoDataAsset.h"

#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAuraAbilityInfoDataAsset::FindAuraAbilityInfoForTag(const FGameplayTag& AbilityTag,
                                                                      const bool bLogNotFound) const
{
	// Search in the available abilities if any has the provided ability tag
	for (const FAuraAbilityInfo& AuraAbilityInfo : AbilityInformation)
	{
		if (AuraAbilityInfo.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return AuraAbilityInfo;
		}
	}
	
	// Display logging if not found and marked for logging
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("No Aura Ability Info found in Ability Info Data Asset [%s] for the Ability Tag [%s]"), 
			*GetNameSafe(this) ,*AbilityTag.ToString());
	}
	return FAuraAbilityInfo();
}
