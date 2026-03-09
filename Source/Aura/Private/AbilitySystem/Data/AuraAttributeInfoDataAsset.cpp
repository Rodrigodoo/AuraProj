


#include "AbilitySystem/Data/AuraAttributeInfoDataAsset.h"

FAuraAttributeInfo UAuraAttributeInfoDataAsset::FindAttributeInfoFromTag(const FGameplayTag& AttributeTag, const bool bLogNotFound) const
{
	// Find the info by tag
	for (const FAuraAttributeInfo& AttributeInfo : AttributeInformation)
	{
		// Needs to be an exact match!
		if (AttributeInfo.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return AttributeInfo;
		}
	}
	
	// No Attribute Info was found!
	if (bLogNotFound)
	{
		// If logging is enabled, display error message
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
		
	}
	return FAuraAttributeInfo();
}
