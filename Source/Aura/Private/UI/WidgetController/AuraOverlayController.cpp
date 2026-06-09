


#include "UI/WidgetController/AuraOverlayController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraAbilityInfoDataAsset.h"
#include "AbilitySystem/Data/AuraLevelUpInfoDataAsset.h"
#include "Player/AuraPlayerState.h"

void UAuraOverlayController::BroadcastInitialValues()
{
	// Broadcast Initial Values
	OnHealthChanged.Broadcast(GetAuraAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAttributeSet()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAttributeSet()->GetMaxMana());
	
	// Broadcast initial XP Percentage
	BroadcastXPPercentageChanges();
}

void UAuraOverlayController::BindCallbacksToDependencies()
{
	// Bind Health and Max Health changes
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
	
	// Bind Mana and Max Mana changes
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
	
	// Note: Make sure this method is only called after UAuraAbilitySystemComponent is valid!
	// Bind callbacks in this controller to the Aura ASC
	// If the Startup Abilities have been initialized and given
	if (GetAuraAbilitySystemComponent()->AreStartupAbilitiesGiven())
	{
		// Retrieve the Ability Information for display
		BroadcastAbilityInfo();
	}
	else
	{
		// Otherwise, Bind to the AbilitiesGivenDelegate to retrieve the Ability Information when available
		GetAuraAbilitySystemComponent()->AbilitiesGivenDelegate.AddUObject(this, &UAuraOverlayController::BroadcastAbilityInfo);
	}


	// Bind to messages
	GetAuraAbilitySystemComponent()->EffectAssetTagsDelegate.AddLambda(
	[this](const FGameplayTagContainer& AssetTags)
		{
			// Debug message showing all tags
			for(const FGameplayTag& Tag : AssetTags)
			{
				// Check if tag belongs to Messages tag
				// How MatchesTag() works: "A.1".MatchesTag("A") will return True, "A".MatchesTag("A.1") will return False
				FGameplayTag MessageGameplayTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageGameplayTag))
				{
					// If it is a message tag then broadcast to listening widgets
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);
	
	// Tell listeners that an Ability has changed status (this might mean a new input tag or status tag)
	GetAuraAbilitySystemComponent()->AbilityStatusChangedDelegate.AddLambda(
		[&](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
		{
			if (AbilityInfoDataAsset)
			{
				// Grab the ability info, update its status tag and broadcast it
				FAuraAbilityInfo AbilityInfo = AbilityInfoDataAsset->FindAuraAbilityInfoForTag(AbilityTag);
				
				// Set the Input tag retrieved from the spec into the data asset entry
				AbilityInfo.InputTag = GetAuraAbilitySystemComponent()->GetInputTagFromAbilityTag(AbilityTag);
				AbilityInfo.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(AbilityInfo);
			}
		});
	
	// Bind to Player State
	// Get any XP changes
	GetAuraPlayerState()->OnPlayerXPChangedDelegate.AddLambda(
			[&](int32 NewXP)
			{
				// Broadcast the new XP level
				BroadcastXPPercentageChanges();
			});
	
	// Get any Level changes
	GetAuraPlayerState()->OnPlayerLevelChangedDelegate.AddLambda(
			[&](const int32 NewLevel)
			{
				// Broadcast the new level
				OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
			});
}

void UAuraOverlayController::BroadcastXPPercentageChanges()
{
	// Note: Make sure this method is only called after AuraPlayerState is valid,
	// And it has valid LevelUpInfoDataAsset !
	if (!GetAuraPlayerState()->LevelUpInfoDataAsset)
	{
		return;
	}
	
	// Find the current XP percentage
	const int32 CurrentXP = GetAuraPlayerState()->GetPlayerXP();
	const float CurrentLevelPercentage = GetAuraPlayerState()->LevelUpInfoDataAsset->FindCurrentLevelPercent(CurrentXP);
	
	// Broadcast it to any listening widget 
	XPPercentageChangedDelegate.Broadcast(CurrentLevelPercentage);
}
