


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTagsManager.h"
#include "AbilitySystem/Abilities/AuraGameplayAbilityBase.h"
#include "Aura/AuraLogChannels.h"
#include "Interaction/AuraPlayerInterface.h"

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
			// Add Dynamic Tags to the Spec we are about to grant
			// Startup Input Tags
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraAbility->StartupInputTag);
			// Set Ability as Equipped
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraGameplayTagsManager::Abilities_Status_Equipped);
			
			// Grant the ability
			GiveAbility(AbilitySpec);
		}
	}
	
	// Broadcast that all abilities have been given
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities)
{
	// Loop through the passive abilities and grant them to the character
	for(const TSubclassOf<UGameplayAbility> Ability : PassiveAbilities)
	{
		// Create an Ability Spec for this ability
		FGameplayAbilitySpec AbilitySpec(Ability, 1);

		// Give the ability and activate only once as they will be active throughout the entire game
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

bool UAuraAbilitySystemComponent::AreStartupAbilitiesGiven() const
{
	return bStartupAbilitiesGiven;
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

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& ForEachAbilityDelegate)
{
	// Lock all the activatable abilities (so they cannot be deactivated) and loop through all of them
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// Call the delegate for this ability, and log if not bound
		if (!ForEachAbilityDelegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__)
		}
	}
}

bool UAuraAbilitySystemComponent::CheckAbilityCostFromTag(const FGameplayTag& AbilityTag)
{
	// Retrieve the Ability Spec from the ability tag and check if it can accept its cost
	if (const FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromTag(AbilityTag))
	{
		return AbilitySpec->Ability->CheckCost(AbilitySpec->Handle, AbilityActorInfo.Get());
	}
	return false;
}

void UAuraAbilitySystemComponent::UpgradeAttributes(const FGameplayTag& AttributeTag)
{
	// Early checks
	if (!GetAvatarActor()->Implements<UAuraPlayerInterface>())
	{
		return;
	}
	if (IAuraPlayerInterface::Execute_GetPlayerAttributePoints(GetAvatarActor()) < 1)
	{
		return;
	}
	
	// Call the upgrade on the server
	ServerUpgradeAttributes_Implementation(AttributeTag, 1);
}

void UAuraAbilitySystemComponent::RevertAttributes(const FGameplayTag& AttributeTag)
{
	// Early checks
	if (!GetAvatarActor()->Implements<UAuraPlayerInterface>())
	{
		return;
	}
	
	// Revert the upgrade on the server
	ServerUpgradeAttributes_Implementation(AttributeTag, -1);
}

void UAuraAbilitySystemComponent::ServerUpgradeAttributes_Implementation(const FGameplayTag& AttributeTag, const int32 AttributeValue)
{
	// Create an event and send it to the Avatar Actor
	// The actor should have a passive ability that waits for events and then applies a change
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = AttributeValue;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
	
	// Decrease the Available Attribute Points by one
	IAuraPlayerInterface::Execute_AddToPlayerAttributePoints(GetAvatarActor(), -AttributeValue);
}

const FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetAbilitySpecFromTag(const FGameplayTag& AbilityTag)
{
	// Loop through the activatable abilities and find which one has this ability tag
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.Ability && Spec.Ability->GetAssetTags().HasTag(AbilityTag))
		{
			return &Spec;
		}
	}
	return nullptr;
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	
	// On the client this will be false the first time
	if (!bStartupAbilitiesGiven)
	{
		// The server already has broadcast this delegate, now should be the client
		// Should only broadcast once
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
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
	EffectAssetTagsDelegate.Broadcast(TagContainer);
}
