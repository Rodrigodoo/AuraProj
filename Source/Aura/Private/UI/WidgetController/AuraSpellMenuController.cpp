


#include "UI/WidgetController/AuraSpellMenuController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AuraAbilityInfoDataAsset.h"
#include "Player/AuraPlayerState.h"

void UAuraSpellMenuController::BroadcastInitialValues()
{
	// Broadcast all initial ability info
	BroadcastAbilityInfo();
	
	// Broadcast the initial spell points
	SpellPointsDelegate.Broadcast(GetAuraPlayerState()->GetSpellPoints());
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
	
	// Bind to changes in the PlayerState's Spell Points
	GetAuraPlayerState()->OnPlayerSpellPointsChangedDelegate.AddLambda(
		[this](const int32 SpellPoints)
		{
			SpellPointsDelegate.Broadcast(SpellPoints);
		});
}

void UAuraSpellMenuController::SelectAbility(UAuraUserWidget* AbilityButton)
{
	AbilitySelectedDelegate.Broadcast(AbilityButton);
}
