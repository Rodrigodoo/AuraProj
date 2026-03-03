	

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	// Called when the Ability Actor Info has been set
	// Use this to bind delegates
	void AbilityActorInfoSet();
	
protected:
	// Method bound to bind to delegate OnGameplayEffectAppliedDelegateToSelf(FOnGameplayEffectAppliedDelegate)
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
	
};
