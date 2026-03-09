
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

//~ Begin Delegates

// Delegate to broadcast Effect Asset tags
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/)
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
	
	// Delegate to broadcast the effect's asset tags via a FGameplayTagContainer
	FEffectAssetTags EffectAssetTags;
	
protected:
	// Method bound to bind to delegate OnGameplayEffectAppliedDelegateToSelf(FOnGameplayEffectAppliedDelegate)
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const;
	
};
