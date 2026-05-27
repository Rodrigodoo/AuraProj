


#include "UI/WidgetController/AuraSpellMenuController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AuraAbilityInfoDataAsset.h"

void UAuraSpellMenuController::BroadcastInitialValues()
{
	// Broadcast all initial ability info
	BroadcastAbilityInfo();
}

void UAuraSpellMenuController::BindCallbacksToDependencies()
{
	GetAuraAbilitySystemComponent()->AbilityStatusChangedDelegate.AddLambda(
		[&](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
		{
			if (AbilityInfoDataAsset)
			{
				// Grab the ability info, update its status tag and broadcast it
				FAuraAbilityInfo AbilityInfo = AbilityInfoDataAsset->FindAuraAbilityInfoForTag(AbilityTag);
				AbilityInfo.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(AbilityInfo);
			}
		});
}
