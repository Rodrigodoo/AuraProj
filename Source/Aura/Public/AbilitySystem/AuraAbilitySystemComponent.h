
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

//~ Begin Delegates

// Delegate to broadcast Effect Asset tags
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/)
// Delegate to broadcast when all abilities have been given
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven)
// Delegate to loop over each given ability
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&)
// Delegate to call whenever an Ability Status changed
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, 
	const FGameplayTag& /*Ability Tag*/, const FGameplayTag&/*Status Tag*/, int32 /*Ability Level*/)
//~ End Delegates
/**
 * The Aura Ability System Component, in charge of dealing with all Gameplay Ability Systems features for this project
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	// Called when the Ability Actor Info has been set
	// Use this to bind delegates
	void AbilityActorInfoSet();
	
	// Add Abilities to the character implementing this component
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);
	
	// Add Passive Abilities to the character implementing this component
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities);
	
	// Check if Startup Abilities have been given
	bool AreStartupAbilitiesGiven() const;
	
	// Called when an Ability Action has been Pressed/Held
	// InputTag - identifies the input Pressed/Held
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	
	// Called when an Ability Action has been released
	// InputTag - identifies the input Released
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
	// Loops through all activatable abilities and passes them to the given delegate
	void ForEachAbility(const FForEachAbility& ForEachAbilityDelegate);
	
	// Checks if the ability can be used (cost check passes) from its Ability Tag
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystem")
	bool CheckAbilityCostFromTag(const FGameplayTag& AbilityTag);

	// Upgrades the attribute related to the tag
	void UpgradeAttributes(const FGameplayTag& AttributeTag);
	
	// Reverts the attribute point related to the tag
	void RevertAttributes(const FGameplayTag& AttributeTag); 
	
	// Server RPC to upgrade the attribute related to the tag
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttributes(const FGameplayTag& AttributeTag, int32 AttributeValue);
	
	// Retrieve a pointer to a gameplay ability spec, with the given ability tag
	// If no such ability is yet activated, then it return null
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	
	// Retrieves the descriptions available for this Ability
	bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);
	
	// Updates by checking them against the level provided
	// Note: should only run on server
	void UpdateAbilityStatuses(int32 Level);
	
	// Server method to tell the Ability System component that a spell point was spent on a specific ability
	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);
	
	// Delegate to broadcast the effect's asset tags via a FGameplayTagContainer
	FEffectAssetTags EffectAssetTagsDelegate;
	
	// Delegate to broadcast when all abilities have been given
	FAbilitiesGiven AbilitiesGivenDelegate;
	
	// Delegate to broadcast whenever and ability status has changed
	FAbilityStatusChanged AbilityStatusChangedDelegate;
	
protected:
	// Method bound to delegate OnGameplayEffectAppliedDelegateToSelf(FOnGameplayEffectAppliedDelegate)
	// This is a Client RPC so that it informs the owning client that an effect was applied (Reliable to make sure it runs on client)
	// This is needed since OnGameplayEffectAppliedDelegateToSelf is only run on the server
	UFUNCTION(Client, Reliable)
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const;
	
	// This is a Client RPC that informs the owning client that an ability status has changed (Reliable to make sure it runs on client)
	// This is needed since UpdateAbilityStatuses is only run on the server
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
	
	// Finds the ability spec from all activatable abilities using the ability's tag 
	const FGameplayAbilitySpec* GetAbilitySpecFromTag(const FGameplayTag& AbilityTag);
	
	//~ Begin - UAbilitySystemComponent Overrides
	// Activatable Abilities replication method
	virtual void OnRep_ActivateAbilities() override;
	
	// Will be called from GiveAbility or from OnRep. Initializes events (triggers and inputs) with the given ability
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;

	//~ End - UAbilitySystemComponent Overrides
	
	// Flag to identify when all startup abilities have been given
    bool bStartupAbilitiesGiven = false;
	
private:
	// Internal Method to Broadcast and Ability Status Update
	void BroadCastAbilityStatusUpdate(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
};
