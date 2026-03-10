


#include "UI/WidgetController/AuraAttributeMenuController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraAttributeInfoDataAsset.h"

void UAuraAttributeMenuController::BroadcastInitialValues()
{
	// Early checks
	check(AuraAttributeInfoDataAsset)
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	// Read the attributes' value from the Attribute Set and apply it to the Attribute Info struct to be broadcast
	// Pair: Key - FGameplayTag | Value - Function pointer (FGameplayAttribute(*)())
	// Note: to get the gameplay attribute from the Value you need to execute the method it's bound to
	for (const auto& Pair : AuraAttributeSet->TagToAttributes)
	{
		// Broadcasts this Attribute Info using the provided TagToAttribute pair
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAuraAttributeMenuController::BindCallbacksToDependencies()
{
	// Early checks
	check(AuraAttributeInfoDataAsset)
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	// Get the Attribute Set and bind callbacks to all attributes
	// Pair: Key - FGameplayTag | Value - Function pointer (FGameplayAttribute(*)())
	// Note: to get the gameplay attribute from the Value you need to execute the method it's bound to
	for (const auto& Pair : AuraAttributeSet->TagToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				// Broadcasts this Attribute Info using the provided TagToAttribute pair
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
			);
	}
}

void UAuraAttributeMenuController::BroadcastAttributeInfo(
	const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	// Get the Attribute Info from the Attribute Info Data Asset
	FAuraAttributeInfo Info = AuraAttributeInfoDataAsset->FindAttributeInfoFromTag(AttributeTag);
				
	// Retrieve the Attribute from its bound method (Attribute = Pair.Value();)
	// Store the attribute's value into the Attribute Info struct
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
				
	// Broadcast Attribute Info to listeners
	AttributeInfoDelegate.Broadcast(Info);
}
