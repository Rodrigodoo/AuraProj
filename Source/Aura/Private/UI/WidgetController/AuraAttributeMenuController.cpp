


#include "UI/WidgetController/AuraAttributeMenuController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraAttributeInfoDataAsset.h"
#include "Player/AuraPlayerState.h"

void UAuraAttributeMenuController::BroadcastInitialValues()
{
	// Early checks
	check(AuraAttributeInfoDataAsset)
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	
	// Read the attributes' value from the Attribute Set and apply it to the Attribute Info struct to be broadcast
	// Pair: Key - FGameplayTag | Value - Function pointer (FGameplayAttribute(*)())
	// Note: to get the gameplay attribute from the Value you need to execute the method it's bound to
	for (const auto& Pair : AuraAttributeSet->TagToAttributes)
	{
		// Broadcasts this Attribute Info using the provided TagToAttribute pair
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	
	// Broadcast the initial attribute points
	AttributePointsDelegate.Broadcast(AuraPlayerState->GetAttributePoints());
}

void UAuraAttributeMenuController::BindCallbacksToDependencies()
{
	// Early checks
	check(AuraAttributeInfoDataAsset)
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	
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
	
	// Bind to changes in the PlayerState's Attribute Points
	AuraPlayerState->OnPlayerAttributePointsChangedDelegate.AddLambda(
		[this](const int32 AttributePoints)
		{
			AttributePointsDelegate.Broadcast(AttributePoints);
		});
}

void UAuraAttributeMenuController::UpgradeAttributes(const FGameplayTag& AttributeTag) const
{
	UAuraAbilitySystemComponent* AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraAbilitySystemComponent->UpgradeAttributes(AttributeTag);
}

void UAuraAttributeMenuController::RevertAttributes(const FGameplayTag& AttributeTag) const
{
	UAuraAbilitySystemComponent* AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraAbilitySystemComponent->RevertAttributes(AttributeTag);
}

void UAuraAttributeMenuController::BroadcastAttributeInfo(
	const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	// Get player state
	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	
	// Get the Attribute Info from the Attribute Info Data Asset
	FAuraAttributeInfo Info = AuraAttributeInfoDataAsset->FindAttributeInfoFromTag(AttributeTag);
				
	// Retrieve the Attribute from its bound method (Attribute = Pair.Value();)
	// Store the attribute's value into the Attribute Info struct
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
				
	// Broadcast Attribute Info to listeners
	AttributeInfoDelegate.Broadcast(Info);
	
	// Broadcast initial attribute points
	AttributePointsDelegate.Broadcast(AuraPlayerState->GetAttributePoints());
	
}
