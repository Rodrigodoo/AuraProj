


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTagsManager.h"
#include "AbilitySystem/Abilities/AuraGameplayAbilityBase.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//~ Begin Bind delegates
	
	// Called on server whenever a GE is applied to self.
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
	//~ End Bind delegates
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	// Loop through the abilities and grant them to the character
	for(const TSubclassOf<UGameplayAbility> Ability : Abilities)
	{
		// Create an Ability Spec for this ability
		FGameplayAbilitySpec AbilitySpec(Ability, 1);

		// If the Ability is of Aura Ability type we can add Startup Input tags to this spec
		if (const UAuraGameplayAbilityBase* AuraAbility = Cast<UAuraGameplayAbilityBase>(AbilitySpec.Ability))
		{
			// Add the Startup Input Tag to the dynamic source tags of the Spec we are about to grant
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraAbility->StartupInputTag);
			
			// Grant the ability
			GiveAbility(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	// Early check
	if (!InputTag.IsValid())
	{
		return;
	}

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// If this Ability Spec does not have this Input tag, continue
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			continue;
		}
		
		// Inform the Ability Spec that this Ability Input was pressed
		AbilitySpecInputPressed(AbilitySpec);
		
		// If the Ability is already active, continue
		if (AbilitySpec.IsActive())
		{
			continue;
		}
		
		// This Ability has the Input tag and is not yet activated
		// So try to activate it
		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	// Early check
	if (!InputTag.IsValid())
	{
		return;
	}
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// If this Ability Spec does not have this Input tag, continue
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			continue;
		}
		
		// Inform the Ability Spec that this Ability Input was released
		AbilitySpecInputReleased(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::EffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const
{
	// Retrieve all the tags associated with this Effect 
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	
	// Broadcast to listeners the Tags associated with this Effect Asset
	//Note: This could be Widget Controllers, etc...
	EffectAssetTags.Broadcast(TagContainer);
}
