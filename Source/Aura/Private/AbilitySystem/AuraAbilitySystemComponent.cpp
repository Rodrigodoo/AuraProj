


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTagsManager.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//~ Begin Bind delegates
	
	// Called on server whenever a GE is applied to self.
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
	//~ End Bind delegates
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	// loop through the abilities and grant them to the character
	for(const TSubclassOf<UGameplayAbility> Ability : Abilities)
	{
		// Create an Ability Spec for this ability
		FGameplayAbilitySpec AbilitySpec(Ability, 1);
		
		// Grant the ability
		//GiveAbility(AbilitySpec);
		
		// Grant Ability and Activate it once
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
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
