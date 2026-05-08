


#include "UI/WidgetController/AuraOverlayController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraAbilityInfoDataAsset.h"
#include "AbilitySystem/Data/AuraLevelUpInfoDataAsset.h"
#include "Player/AuraPlayerState.h"

void UAuraOverlayController::BroadcastInitialValues()
{
	// Note: Make sure this method is only called after UAuraAttributeSet is valid!
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	// Broadcast Initial Values
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
	
	// Broadcast initial XP Percentage
	BroadcastXPPercentageChanges();
}

void UAuraOverlayController::BindCallbacksToDependencies()
{
	// Note: Make sure this method is only called after UAuraAttributeSet and the Player are valid!
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	
	// Bind Health and Max Health changes
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
	
	// Bind Mana and Max Mana changes
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
	
	// Note: Make sure this method is only called after UAuraAbilitySystemComponent is valid!
	// Bind callbacks in this controller to the Aura ASC
	if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		// If the Startup Abilities have been initialized and given
		if (AuraAbilitySystemComponent->AreStartupAbilitiesGiven())
		{
			// Retrieve the Ability Information for display
			OnInitializedStartupAbilities();
		}
		else
		{
			// Otherwise, Bind to the AbilitiesGivenDelegate to retrieve the Ability Information when available
			AuraAbilitySystemComponent->AbilitiesGivenDelegate.AddUObject(this, &UAuraOverlayController::OnInitializedStartupAbilities);
		}


		// Bind to messages
		AuraAbilitySystemComponent->EffectAssetTagsDelegate.AddLambda(
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
	}
	
	// Bind to Player State
	// Get any XP changes
	AuraPlayerState->OnPlayerXPChangedDelegate.AddLambda(
			[&](int32 NewXP)
			{
				// Broadcast the new XP level
				BroadcastXPPercentageChanges();
			});
}

void UAuraOverlayController::OnInitializedStartupAbilities()
{
	UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!AuraAbilitySystemComponent || !AuraAbilitySystemComponent->AreStartupAbilitiesGiven())
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
			// Retrieve the Ability Tag from the Spec and search for it in the Ability Info Data Asset
			FAuraAbilityInfo AbilityInfo = AbilityInfoDataAsset->FindAuraAbilityInfoForTag(
				UAuraAbilitySystemLibrary::GetAbilityTagFromSpec(AbilitySpec));
			
			// Set the Input Tag retrieved from the spec into the data asset entry
			AbilityInfo.InputTag = UAuraAbilitySystemLibrary::GetInputTagFromSpec(AbilitySpec);
			
			// Broadcast to listening widgets the Ability Info 
			AbilityInfoDelegate.Broadcast(AbilityInfo);
		});
	
	// Call the looping method and provide it with its delegate
	AuraAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UAuraOverlayController::BroadcastXPPercentageChanges()
{
	// Note: Make sure this method is only called after AuraPlayerState is valid,
	// And it has valid LevelUpInfoDataAsset !
	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	if (!AuraPlayerState->LevelUpInfoDataAsset)
	{
		return;
	}
	
	// Find the current XP percentage
	const int32 CurrentXP = AuraPlayerState->GetPlayerXP();
	const float CurrentLevelPercentage = AuraPlayerState->LevelUpInfoDataAsset->FindCurrentLevelPercent(CurrentXP);
	
	// Broadcast it to any listening widget 
	XPPercentageChangedDelegate.Broadcast(CurrentLevelPercentage);
}
