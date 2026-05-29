


#include "UI/WidgetController/AuraSpellMenuController.h"

#include "AuraGameplayTagsManager.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AuraAbilityInfoDataAsset.h"
#include "Aura/AuraLogChannels.h"
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
		[&](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
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
	// Broadcast to all listening widgets whenever this ability has been selected
	AbilitySelectedDelegate.Broadcast(AbilityButton);
}

void UAuraSpellMenuController::SpendSpellPoint(const FGameplayTag& AbilityTag)
{
	GetAuraAbilitySystemComponent()->ServerSpendSpellPoint(AbilityTag);
}

void UAuraSpellMenuController::ShouldEnableSpendPointAndEquipButtons(const FGameplayTag& StatusTag,
                                                                     bool& bSpendPointEnabled, bool& bEquipEnabled)
{
	// If the status of the ability is Unlocked or Equipped then the buttons to spend points or to equip should be enabled
	// If the Ability is Eligible then only the spend points button should be enabled
	// in all other cases both buttons should be disabled
	bSpendPointEnabled = false;
	bEquipEnabled = false;
	if (StatusTag.MatchesTagExact(AuraGameplayTagsManager::Abilities_Status_Equipped) || 
		StatusTag.MatchesTagExact(AuraGameplayTagsManager::Abilities_Status_Unlocked))
	{
		bSpendPointEnabled = true;
		bEquipEnabled = true;
	}
	else if (StatusTag.MatchesTagExact(AuraGameplayTagsManager::Abilities_Status_Eligible))
	{
		bSpendPointEnabled = true;
	}
}
