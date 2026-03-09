


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTagsManager.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//~ Begin Bind delegates
	
	// Called on server whenever a GE is applied to self.
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
	//~ End Bind delegates
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const
{
	// Retrieve all the tags associated with this Effect 
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	
	// Broadcast to listeners the Tags associated with this Effect Asset
	//Note: This could be Widget Controllers, etc...
	EffectAssetTags.Broadcast(TagContainer);
}
