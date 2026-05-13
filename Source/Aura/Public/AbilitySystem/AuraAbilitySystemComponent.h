
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
	
	// Delegate to broadcast the effect's asset tags via a FGameplayTagContainer
	FEffectAssetTags EffectAssetTagsDelegate;
	
	// Delegate to Broadcast when all abilities have been given
	FAbilitiesGiven AbilitiesGivenDelegate;
	
protected:
	// Method bound to delegate OnGameplayEffectAppliedDelegateToSelf(FOnGameplayEffectAppliedDelegate)
	// This is a Client RPC so that it informs the owning client that an effect was applied (Reliable to make sure it runs on client)
	// This is needed since OnGameplayEffectAppliedDelegateToSelf is only run on the server
	UFUNCTION(Client, Reliable)
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const;
	
	// Finds the ability spec from all activatable abilities using the ability's tag 
	const FGameplayAbilitySpec* GetAbilitySpecFromTag(const FGameplayTag& AbilityTag);
	
	//~ Begin - UAbilitySystemComponent Overrides
	// Activatable Abilities replication method
	virtual void OnRep_ActivateAbilities() override;
	//~ End - UAbilitySystemComponent Overrides
	
	// Flag to identify when all startup abilities have been given
    bool bStartupAbilitiesGiven = false;
};
