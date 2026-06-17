


#include "AbilitySystem/Debuff/AuraDebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/AuraCombatInterface.h"

UAuraDebuffNiagaraComponent::UAuraDebuffNiagaraComponent()
{
	// Do not activate immediately 
	bAutoActivate = false;
}

void UAuraDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the Ability System component from the owner
	// If it is not yet valid wait for the broadcast
	IAuraCombatInterface* AuraCombatInterface = Cast<IAuraCombatInterface>(GetOwner());
	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	// There is already a valid Ability System component
	if (AbilitySystemComponent)
	{
		// Bind tag changes to DebuffTagChanged
		AbilitySystemComponent->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &UAuraDebuffNiagaraComponent::DebuffTagChanged);
	}
	
	// Bind methods to the Combat Interface
	if (AuraCombatInterface)
	{
		// If there is not yet a valid AbilitySystemComponent, bind to the Combat Interface Delegate, and wait for a valid one
		if (!AbilitySystemComponent)
		{
			// Wait for validity broadcast and bind it 
			AuraCombatInterface->GetAbilitySystemComponentRegisteredDelegate().AddWeakLambda(this,
				[&](UAbilitySystemComponent* AbilitySystemComponent)
				{
					// Bind tag changes to DebuffTagChanged
					AbilitySystemComponent->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
						this, &UAuraDebuffNiagaraComponent::DebuffTagChanged);
				});
		}
		
		// Bind on character death
		AuraCombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuraDebuffNiagaraComponent::OnOwnerDeath);
	}
	
	
}

void UAuraDebuffNiagaraComponent::DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	// Check if the owner is alive before activating
	const AActor* Owner = GetOwner();
	const bool bOwnerAlive = IsValid(Owner) && 
		Owner->Implements<UAuraCombatInterface>() && !IAuraCombatInterface::Execute_IsDead(Owner);
	
	if (bOwnerAlive && NewCount > 0)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UAuraDebuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
