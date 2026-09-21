


#include "AbilitySystem/Passive/AuraPassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/AuraCombatInterface.h"

UAuraPassiveNiagaraComponent::UAuraPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UAuraPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind to passive ability status change on the Ability System Component of the owner
	if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = 
		Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		AuraAbilitySystemComponent->ActivatePassiveEffectDelegate.AddUObject(this, &UAuraPassiveNiagaraComponent::OnPassiveActivate);
	}
	else if (IAuraCombatInterface* CombatInterface = Cast<IAuraCombatInterface>(GetOwner()))
	{
		CombatInterface->GetAbilitySystemComponentRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* AbilitySystemComponent)
		{
			if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
			{
				AuraASC->ActivatePassiveEffectDelegate.AddUObject(this, &UAuraPassiveNiagaraComponent::OnPassiveActivate);
			}
		});
	}
}

void UAuraPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
	// If the Ability tag does not match this component's passive tag then return early
	if (!AbilityTag.MatchesTagExact(PassiveSpellTag))
	{
		return;
	}
	
	// If the passive was activated and the niagara component is not yet active, then activate it
	if (bActivate && !IsActive())
	{
		Activate();
	}
	else
	{
		// Otherwise, Deactivate
		Deactivate();
	}
}
