


#include "Input/AuraInputConfigDataAsset.h"

const UInputAction* UAuraInputConfigDataAsset::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	const bool bLogNotFound) const
{
	// Loop through the AbilityInputActions and find if there is an Input Action associated with the given Gameplay Tag
	for (const auto& [InputAction, InputTag] : AbilityInputActions)
	{
		if (InputAction && InputTag.MatchesTagExact(InputTag))
		{
			// If a match was found return it
			return InputAction;
		}
	}
	
	// No match was found
	if (bLogNotFound)
	{
		// Print to log the warning
		UE_LOG(LogTemp, Error, TEXT("No Ability Input Action was found for InputTag [%s], on InputConfig [%s]"), 
			*InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
