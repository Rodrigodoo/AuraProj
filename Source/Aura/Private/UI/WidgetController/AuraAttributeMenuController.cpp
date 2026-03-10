


#include "UI/WidgetController/AuraAttributeMenuController.h"

#include "AuraGameplayTagsManager.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraAttributeInfoDataAsset.h"

void UAuraAttributeMenuController::BroadcastInitialValues()
{
	// Early checks
	check(AuraAttributeInfoDataAsset)
	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	if(!AuraAttributeSet)
	{
		// The Attribute Set is not of the valid type
		return;
	}
	
	// Read the attributes' value from the Attribute Set and apply it to the Attribute Info struct to be broadcast
	// Pair: Key - FGameplayTag | Value - FAttributeSignature (FGameplayAttribute)
	// Note: to get the gameplay attribute from the Value you need to execute the method it's bound to
	for (const auto& Pair : AuraAttributeSet->TagToAttributes)
	{
		// Get the Attribute Info from the Attribute Info Data Asset
		FAuraAttributeInfo Info = AuraAttributeInfoDataAsset->FindAttributeInfoFromTag(Pair.Key);
		
		// Retrieve the Attribute from its bound method (Attribute = Pair.Value();)
		// Store the attribute's value into the Attribute Info struct
		Info.AttributeValue = Pair.Value().GetNumericValue(AuraAttributeSet);
		
		// Broadcast Attribute Info to listeners
		AttributeInfoDelegate.Broadcast(Info);
	}
}

void UAuraAttributeMenuController::BindCallbacksToDependencies()
{
	
}
