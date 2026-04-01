

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 * Aura's global data for the ability system
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
public:
	//~ Begin - UAbilitySystemGlobals overrides
	
	// Should allocate a project specific GameplayEffectContext struct. Caller is responsible for deallocation
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
	//~ End - UAbilitySystemGlobals overrides
};
