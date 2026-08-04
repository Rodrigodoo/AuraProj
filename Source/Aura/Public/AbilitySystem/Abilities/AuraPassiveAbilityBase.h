

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbilityBase.h"
#include "AuraPassiveAbilityBase.generated.h"

/**
 * Passive ability base class (to be used by passive spell abilities)
 */
UCLASS()
class AURA_API UAuraPassiveAbilityBase : public UAuraGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	//~ Begin - UGameplayAbility Overrides
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	//~ End - UGameplayAbility Overrides
	
	// Method to deactivate the ability (to be bound to the Ability System Component)
	void ReceiveDeactivate(const FGameplayTag& PassiveAbilityTag);
};
