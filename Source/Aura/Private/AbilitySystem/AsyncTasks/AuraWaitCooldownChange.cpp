


#include "AbilitySystem/AsyncTasks/AuraWaitCooldownChange.h"

#include "AbilitySystemComponent.h"

UAuraWaitCooldownChange* UAuraWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* InAbilitySystemComponent,
                                                                        const FGameplayTag InCooldownTag)
{
	// Early exit
	if (!IsValid(InAbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		return nullptr;
	}
	
	// Create the new object and cache its Ability System Component and Cooldown Tag
	UAuraWaitCooldownChange* WaitCooldownChange = NewObject<UAuraWaitCooldownChange>();
	WaitCooldownChange->AbilitySystemComponent = InAbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	// Bind the newly created object to any change on the cooldown tag count (add or remove)
	InAbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).
	AddUObject(WaitCooldownChange, &UAuraWaitCooldownChange::CooldownTagChanged);
	
	// Return the new task object
	return WaitCooldownChange;
}

void UAuraWaitCooldownChange::EndTask()
{
	// Early check
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}
	
	// Remove this callback from the delegate list
	AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	
	// Unregister action from game instance and make it ready to destroy
	SetReadyToDestroy();
}

void UAuraWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	// If the tag was removed, then the cooldown ended
	if (NewCount == 0)
	{
		// No more time remaining
		CooldownEnd.Broadcast(0.f);
		return;
	}
	
	// If a Cooldown tag was added then broadcast its time remaining 
	
	// Make a query to check on all active effects if they include this cooldown tag
	const FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());
	// Check all active effects with the cooldown tag what is their cooldown time remaining
	TArray<float> TimesRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemaining(CooldownQuery);
	
	// Early check
	if (TimesRemaining.IsEmpty())
	{
		return;
	}
	
	// Only one cooldown should be active at the time
	// But in case it's not, using this safeguard to retrieve the largest available cooldown time remaining
	float TimeRemaining = TimesRemaining[0];
	for (const float Time : TimesRemaining)
	{
		if (Time > TimeRemaining)
		{
			TimeRemaining = Time;
		}
	}
	
	// Broadcast Time remaining for this cooldown
	CooldownStart.Broadcast(TimeRemaining);
	
}
