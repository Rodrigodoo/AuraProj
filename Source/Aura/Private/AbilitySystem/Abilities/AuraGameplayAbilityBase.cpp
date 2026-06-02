


#include "AbilitySystem/Abilities/AuraGameplayAbilityBase.h"

#include "AbilitySystem/AuraAttributeSet.h"

float UAuraGameplayAbilityBase::GetManaCost(const int32 Level) const
{
	// Get the cost gameplay effect and go through its modifiers to check the cost of this ability
	float CostValue = 0.f;
	UGameplayEffect* CostGE = GetCostGameplayEffect();
	if (!CostGE)
	{
		return CostValue;
	}
	
	// Iterate through the modifiers to find the cost
	for (const FGameplayModifierInfo& ModInfo : CostGE->Modifiers)
	{
		// Only count costs to Mana
        if (ModInfo.Attribute != UAuraAttributeSet::GetManaAttribute())
        {
	        continue;
        }

		// If it has mana modifier then retrieve its magnitude value (Needs to be a scalable float)
		ModInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, CostValue);
		break;
	}
	
	// Return the cost
	return FMath::Abs(CostValue);
}

float UAuraGameplayAbilityBase::GetCooldown(const int32 Level) const
{
	// Get the cooldown gameplay effect check its duration if it has one
	float CooldownValue = 0.f;
	const UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (!CooldownGE)
	{
		return CooldownValue;
	}

	// If the cooldown has no duration then return early
	if (CooldownGE->DurationPolicy != EGameplayEffectDurationType::HasDuration)
	{
		return CooldownValue;
	}
	
	// If it has duration then return its value (Needs to be a scalable float)
	CooldownGE->DurationMagnitude.GetStaticMagnitudeIfPossible(Level, CooldownValue);
	
	return CooldownValue;
}
