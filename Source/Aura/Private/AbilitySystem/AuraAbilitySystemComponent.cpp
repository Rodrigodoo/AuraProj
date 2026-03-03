


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//~ Begin Bind delegates
	
	// Called on server whenever a GE is applied to self.
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
	//~ End Bind delegates
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	// Retrieve all the tags associated with this Effect 
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	
	// Broadcast tags to Widget Controller to display info to player
	for(const FGameplayTag& Tag : TagContainer)
	{
		// TODOBARBORO - Broadcast tag to WidgetController
		FString TagName = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
		GEngine->AddOnScreenDebugMessage(-1,8.f, FColor::Blue, TagName);
	}
	
}
