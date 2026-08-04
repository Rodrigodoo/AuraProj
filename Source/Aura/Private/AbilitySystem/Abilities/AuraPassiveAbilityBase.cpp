


#include "AbilitySystem/Abilities/AuraPassiveAbilityBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraPassiveAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	// Call Super Before
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Get the Aura Ability System Component and bind to its deactivate ability
	// Note: do not rebind the ability if it has already been done
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (AuraASC && !AuraASC->DeactivatePassiveAbilityDelegate.IsBoundToObject(this))
	{
		AuraASC->DeactivatePassiveAbilityDelegate.AddUObject(this, &UAuraPassiveAbilityBase::ReceiveDeactivate);
	}
}

void UAuraPassiveAbilityBase::ReceiveDeactivate(const FGameplayTag& PassiveAbilityTag)
{
	// If this is the ability to deactivate, then End the Ability 
	if (GetAssetTags().HasTagExact(PassiveAbilityTag))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}
