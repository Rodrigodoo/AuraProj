


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTagsManager.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/AuraGameplayAbilityBase.h"
#include "AbilitySystem/Data/AuraAbilityInfoDataAsset.h"
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

void UAuraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	// Early check
	if (!InputTag.IsValid())
	{
		return;
	}

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// If this Ability Spec does not have this Input tag, continue
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			continue;
		}
		
		// Inform the Ability Spec that this Ability Input was pressed
		AbilitySpecInputPressed(AbilitySpec);
		
		// If the Ability is not active, continue
		if (!AbilitySpec.IsActive())
		{
			continue;
		}
		
		// Need to Invoke the Replicated Event (input pressed)
		TArray<UGameplayAbility*> AbilityInstances = AbilitySpec.GetAbilityInstances();
		for (const UGameplayAbility* AbilityInstance : AbilityInstances)
		{
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle,
				AbilityInstance->GetCurrentActivationInfo().GetActivationPredictionKey());
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

	FScopedAbilityListLock ActiveScopeLock(*this);
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
	
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// If this Ability Spec does not have this Input tag, continue
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			continue;
		}

		// Need to Invoke the Replicated Event (input released)
		if (AbilitySpec.IsActive())
		{
			// Inform the Ability Spec that this Ability Input was released
			AbilitySpecInputReleased(AbilitySpec);
			
			TArray<UGameplayAbility*> AbilityInstances = AbilitySpec.GetAbilityInstances();
			for (const UGameplayAbility* AbilityInstance : AbilityInstances)
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilityInstance->GetCurrentActivationInfo().GetActivationPredictionKey());
			}
		}
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

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	// Lock all activatable abilities while looping through them
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// Check if any of the activatable abilities has the provided ability tag
		if (AbilitySpec.Ability->IsValidLowLevel() && AbilitySpec.Ability->GetAssetTags().HasTagExact(AbilityTag))
		{
			// If it does, then return it
			return &AbilitySpec;
		}
	}
	// None was found
	return nullptr;
}

bool UAuraAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription,
	FString& OutNextLevelDescription)
{
	// Early checks
	if (!AbilityTag.IsValid())
	{
		OutDescription = "Error - No Ability Tag Set!";
		OutNextLevelDescription = "";
		return false;
	}
	
	// Check if ability is unlocked
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		// Get the descriptions from the Ability Info Data Asset
		// Note: using the Ability Spec Level as the Ability might not yet been updated
		return UAuraAbilitySystemLibrary::GetAbilityDescriptions(
			GetAvatarActor(), AbilityTag, OutDescription, OutNextLevelDescription, AbilitySpec->Level);
	}
	
	// Since the ability is locked (or no valid ability was found), get the locked description out via the Out Description
	// Note: the level here makes no difference
	return UAuraAbilitySystemLibrary::GetAbilityDescriptions(
			GetAvatarActor(), AbilityTag, OutDescription, OutNextLevelDescription, 
			1, true);
}

void UAuraAbilitySystemComponent::ClearInputTagFromAbilities(const FGameplayTag& InputTag)
{
	// Lock all activatable abilities while looping through them
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// If the Ability uses this input tag, then clear it.
		if (UAuraAbilitySystemLibrary::AbilityHasInputTag(AbilitySpec, InputTag))
		{
			UAuraAbilitySystemLibrary::ClearInputTagFromSpec(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::UpdateAbilityStatuses(const int32 Level)
{
	// Running on server
	UAuraAbilityInfoDataAsset* AbilityInfoDataAsset = UAuraAbilitySystemLibrary::GetAbilityInfoDataAsset(GetAvatarActor());
	if (!AbilityInfoDataAsset)
	{
		// Early check
		return;
	}

	// loop through all abilities and change their status according to the provided level
	for (const FAuraAbilityInfo& AbilityInfo : AbilityInfoDataAsset->AbilityInformation)
	{
		// If the level has not yet reached the level requirement, 
		// Or the Ability Tag is not valid, then continue
		if (Level < AbilityInfo.LevelRequirement || !AbilityInfo.AbilityTag.IsValid())
		{
			continue;
		}
		
		// Only update the status for abilities that have NOT yet been activated
		// Non activated abilities will return as nullptr to GetSpecFromAbilityTag
		if (!GetSpecFromAbilityTag(AbilityInfo.AbilityTag))
		{
			// Make an ability spec, mark it as eligible and give the ability
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityInfo.AbilityClass , 1);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraGameplayTagsManager::Abilities_Status_Eligible);

			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec); // Make the ability spec replicate immediately
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag);
	if (!AbilitySpec)
	{
		return FGameplayTag();
	}
	return UAuraAbilitySystemLibrary::GetInputTagFromSpec(*AbilitySpec);
}

void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,
                                                                    const FGameplayTag& InputTag)
{
	// Get the Ability Spec to find its status. If it is locked (not activatable) then return
	FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag);
	if (!AbilitySpec)
	{
		return;
	}
	
	// Retrieve the old InputTag and the status
	const FGameplayTag& PreviousInputTag = UAuraAbilitySystemLibrary::GetInputTagFromSpec(*AbilitySpec);
	const FGameplayTag& Status = UAuraAbilitySystemLibrary::GetStatusTagFromSpec(*AbilitySpec);
	
	// If the ability is Locked (should not happen here) or Eligible (No yet purchased)
	// Then return as it should not be able to be equipped
	if (Status.MatchesTagExact(AuraGameplayTagsManager::Abilities_Status_Locked) || 
		Status.MatchesTagExact(AuraGameplayTagsManager::Abilities_Status_Eligible))
	{
		return;
	}
	
	// Handle activation/deactivation Passive abilities
	
	// Find if any ability spec is using this input tag (slot)
	FGameplayAbilitySpec* SpecWithInputTag = GetAbilitySpecWithInputTag(InputTag);
	if (SpecWithInputTag)
	{
		// If the spec with this Input Tag is the same ability that was there before, then return early
		const FGameplayTag& AbilityTagInSlot = UAuraAbilitySystemLibrary::GetAbilityTagFromSpec(*SpecWithInputTag);
		if (AbilityTag.MatchesTagExact(AbilityTagInSlot))
		{
			// This should update the UI (even if there was no change to the Status)
			ClientUpdateAbilityStatus(AbilityTag, InputTag, AuraGameplayTagsManager::Abilities_Status_Equipped, AbilitySpec->Level);
			return;
		}

		// If it is a passive ability different from the new one, then deactivate it
		if (UAuraAbilitySystemLibrary::IsPassiveAbility(GetAvatarActor(), *AbilitySpec))
		{
			DeactivatePassiveAbilityDelegate.Broadcast(AbilityTagInSlot);
			UAuraAbilitySystemLibrary::ClearInputTagFromSpec(*SpecWithInputTag);
		}
	}

	// Ability does not yet have an Input Tag (It's not yet active)
	// And it is a passive ability
	if (!UAuraAbilitySystemLibrary::AbilityHasAnyInputTag(*AbilitySpec) && 
		UAuraAbilitySystemLibrary::IsPassiveAbility(GetAvatarActor(), *AbilitySpec))
	{
		// Then try to activate it
		TryActivateAbility(AbilitySpec->Handle);
	}
	
	// Reassign the Input Tag to the new Ability Spec
	
	// Clear this Input Tag from all abilities (only one should have it but this is a fail-safe)
	ClearInputTagFromAbilities(InputTag);
	
	// Assign the new Input Tag to the Ability Spec
	UAuraAbilitySystemLibrary::AssignInputTagToSpec(*AbilitySpec, InputTag);

	// If the Ability was unlocked, convert its status to Equipped
	if (Status.MatchesTagExact(AuraGameplayTagsManager::Abilities_Status_Unlocked))
	{
		AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(Status);
		AbilitySpec->GetDynamicSpecSourceTags().AddTag(AuraGameplayTagsManager::Abilities_Status_Equipped);
	}
	
	// Mark Spec as dirty to immediately replicate changes
	MarkAbilitySpecDirty(*AbilitySpec);
	
	// Now Broadcast the change in status
	// This should update the UI
	ClientUpdateAbilityStatus(AbilityTag, InputTag, AuraGameplayTagsManager::Abilities_Status_Equipped, AbilitySpec->Level);
}

void UAuraAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	// Get the Ability Spec to find its status. If it is locked (not activatable) then return
	FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag);
	if (!AbilitySpec)
	{
		// Early exit
		return;
	}
	
	// Get the status of the currently provided ability
	// Since it passed the GetSpecFromAbilityTag it means that it is an activatable Ability
	// That means it will be either: Unlocked, Eligible or Equipped.
	FGameplayTag StatusTag = UAuraAbilitySystemLibrary::GetStatusTagFromSpec(*AbilitySpec);
	if (StatusTag.MatchesTagExact(AuraGameplayTagsManager::Abilities_Status_Locked))
	{
		// Something went wrong as the status tag should never reach here as Abilities_Status_Locked
		UE_LOG(LogAura, Error, TEXT("The [%s] Ability has a [%s] status tag. "
							  "This should not happen for an activatable Ability]"), 
							  *AbilityTag.ToString(), *StatusTag.ToString());
		return;
	}
	
	// If the status is eligible, purchase the ability
	// Note: Make sure you only have one status tag at the time. The system depends on that guarantee
	if (StatusTag.MatchesTagExact(AuraGameplayTagsManager::Abilities_Status_Eligible))
	{
		// Remove the old status tag and add the new (Old: Eligible / New: Unlocked)
		AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(StatusTag);
		AbilitySpec->GetDynamicSpecSourceTags().AddTag(AuraGameplayTagsManager::Abilities_Status_Unlocked);
		StatusTag = AuraGameplayTagsManager::Abilities_Status_Unlocked;
	} 
	// If the status is either equipped or unlocked, upgrade the ability
	else if (StatusTag.MatchesTagExact(AuraGameplayTagsManager::Abilities_Status_Equipped) || 
		StatusTag.MatchesTagExact(AuraGameplayTagsManager::Abilities_Status_Unlocked))
	{
		// No need to change the status tag
		// Just level up the ability
		AbilitySpec->Level += 1;
	}
	
	// Spend a spell point
	IAuraPlayerInterface::Execute_AddToPlayerSpellPoints(GetAvatarActor(), -1);
	
	// Inform the client UI that the Ability status has changed
	ClientUpdateAbilityStatus(AbilityTag, UAuraAbilitySystemLibrary::GetInputTagFromSpec(*AbilitySpec), StatusTag, AbilitySpec->Level);
	MarkAbilitySpecDirty(*AbilitySpec);
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

void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
                                                                           const FGameplayTag& InputTag, const FGameplayTag& StatusTag, const int32 AbilityLevel)
{
	// Broadcast that this ability changed status
	BroadcastAbilityStatusUpdate(AbilityTag, InputTag, StatusTag, AbilityLevel);
}

const FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetAbilitySpecFromTag(const FGameplayTag& AbilityTag)
{
	// Loop through the activatable abilities and find which one has this ability tag
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.Ability && Spec.Ability->GetAssetTags().HasTag(AbilityTag))
		{
			return &Spec;
		}
	}
	return nullptr;
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetAbilitySpecWithInputTag(const FGameplayTag& InputTag)
{
	// Loop through the activatable abilities and find which one has this input tag
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (UAuraAbilitySystemLibrary::AbilityHasInputTag(Spec, InputTag))
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

void UAuraAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	// Call super before doing our logic
	Super::OnGiveAbility(AbilitySpec);
	
	// Note: Only go forward if we are dealing with locally controlled actors, as we use it to update UI elements
	if (!AbilityActorInfo->IsLocallyControlled())
	{
		return;
	}
	
	// If an Ability just became Eligible (i.e. like due to a level up)
	// Broadcast the change of the Ability Status
	const FGameplayTag StatusTag = UAuraAbilitySystemLibrary::GetStatusTagFromSpec(AbilitySpec);
	if (StatusTag.MatchesTagExact(AuraGameplayTagsManager::Abilities_Status_Eligible))
	{
		// Broadcast that this ability changed status
		BroadcastAbilityStatusUpdate(
			UAuraAbilitySystemLibrary::GetAbilityTagFromSpec(AbilitySpec),
			UAuraAbilitySystemLibrary::GetInputTagFromSpec(AbilitySpec), StatusTag, AbilitySpec.Level);
	}
	
}

void UAuraAbilitySystemComponent::BroadcastAbilityStatusUpdate(const FGameplayTag& AbilityTag,
                                                               const FGameplayTag& InputTag, const FGameplayTag& StatusTag, const int32 AbilityLevel)
{
	// Broadcast that this ability changed status
	AbilityStatusChangedDelegate.Broadcast(AbilityTag, InputTag, StatusTag, AbilityLevel);
}

void UAuraAbilitySystemComponent::EffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                               const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const
{
	// Retrieve all the tags associated with this Effect 
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	
	// Broadcast to listeners the Tags associated with this Effect Asset
	// Note: This could be Widget Controllers, etc...
	EffectAssetTagsDelegate.Broadcast(TagContainer);
}
