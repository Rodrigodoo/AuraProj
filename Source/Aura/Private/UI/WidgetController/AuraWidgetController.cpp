


#include "UI/WidgetController/AuraWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraAbilityInfoDataAsset.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
	// Base Class does no work. Implement in child class!
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
	// Base Class does no work. Implement in child class!
}

void UAuraWidgetController::BroadcastAbilityInfo()
{
	if (!GetAuraAbilitySystemComponent()->AreStartupAbilitiesGiven())
	{
		return;
	}
	
	// Loop through all activatable Abilities and retrieve their asset tags and input tags
	// This will be used to find the appropriate Ability Info and set its Input Tag, 
	// then inform all listening widgets of this Ability's information 
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
		[this](const FGameplayAbilitySpec& AbilitySpec)
		{
			// Retrieve the Ability Tag from the Spec
			const FGameplayTag AbilityTag = UAuraAbilitySystemLibrary::GetAbilityTagFromSpec(AbilitySpec);
			if (!AbilityTag.IsValid())
			{
				// Early exit
				return;
			}
			
			// Use the Ability tag to search for the Ability Info in the Ability Info Data Asset
			FAuraAbilityInfo AbilityInfo = AbilityInfoDataAsset->FindAuraAbilityInfoForTag(AbilityTag);

			// Set the Input Tag retrieved from the spec into the data asset entry
			AbilityInfo.InputTag = UAuraAbilitySystemLibrary::GetInputTagFromSpec(AbilitySpec);
			
			// Broadcast to listening widgets the Ability Info 
			AbilityInfoDelegate.Broadcast(AbilityInfo);
		});
	
	// Call the looping method and provide it with its delegate
	GetAuraAbilitySystemComponent()->ForEachAbility(BroadcastDelegate);
}

AAuraPlayerController* UAuraWidgetController::GetAuraPlayerController()
{
	if (!AuraPlayerController)
	{
		AuraPlayerController = CastChecked<AAuraPlayerController>(PlayerController);
	}
	return AuraPlayerController;
}

AAuraPlayerState* UAuraWidgetController::GetAuraPlayerState()
{
	if (!AuraPlayerState)
	{
		AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	}
	return AuraPlayerState;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraAbilitySystemComponent()
{
	if (!AuraAbilitySystemComponent)
	{
		AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	}
	return AuraAbilitySystemComponent;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAttributeSet()
{
	if (!AuraAttributeSet)
	{
		AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	}
	return AuraAttributeSet;
}
