


#include "AbilitySystem/Data/AuraAbilityInfoDataAsset.h"

#include "AuraGameplayTagsManager.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AbilitySystem/Abilities/AuraGameplayAbilityBase.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Aura/AuraLogChannels.h"

namespace AuraRichStyle
{
	// Base Ability Markups
	FString Level = "_Level";
	FString ManaCost = "_ManaCost";
	FString Cooldown = "_Cooldown";
	
	// Damage Ability Markups
	FString FireDmg = "_FireDmg";
	FString LightningDmg = "_LightningDmg";
	FString ArcaneDmg = "_ArcaneDmg";
	FString PhysicalDmg = "_PhysicalDmg";
	
	// Projectile Spell Ability Markups
	FString ProjectileNum = "_ProjectileNum";
}

FString FAuraAbilityInfo::GetDescription(const int32 Level) const
{
	FString DescriptionCopy = Description;
	FormatTextValues(DescriptionCopy, Level);
	return DescriptionCopy;
}

FString FAuraAbilityInfo::GetNextLevelDescription(const int32 Level) const
{
	FString NextLevelDescriptionCopy = NextLevelDescription;
	FormatTextValues(NextLevelDescriptionCopy, Level,true);
	return NextLevelDescriptionCopy;
}

FString FAuraAbilityInfo::GetLockedDescription() const
{
	return FString::Printf(TEXT("Spell Locked until level:<Level>%d</>"), LevelRequirement);
}

void FAuraAbilityInfo::FormatTextValues(FString& OutText, const int32 Level, const bool bNextLevel) const
{
	UAuraGameplayAbilityBase* AbilityDefault = Cast<UAuraGameplayAbilityBase>(AbilityClass.GetDefaultObject());
	if (!AbilityDefault)
	{
		// Failed to retrieve a valid Aura Ability
		return;
	}

	// Get the according level
	const int32 AbilityLevel = bNextLevel ? Level + 1 : Level;
	
	// Map to replace names(identifiers) with their respective values
	FStringFormatNamedArguments NamesToValues;
	
	// Level, Next Level, Mana Cost and Cooldown of Ability
	NamesToValues.Add(AuraRichStyle::Level, FStringFormatArg(AbilityLevel));
	NamesToValues.Add(AuraRichStyle::ManaCost, 
		FStringFormatArg(FString::SanitizeFloat(AbilityDefault->GetManaCost(AbilityLevel))));
	NamesToValues.Add(AuraRichStyle::Cooldown, 
		FStringFormatArg(FString::SanitizeFloat(AbilityDefault->GetCooldown(AbilityLevel))));

	// If the Ability is a damage ability
	if (const UAuraDamageGameplayAbility* DamageGameplayAbility = Cast<UAuraDamageGameplayAbility>(AbilityDefault))
	{
		// Get all the damage types
		float Damage = DamageGameplayAbility->GetDamageAtLevel(AuraGameplayTagsManager::Damage_Fire, AbilityLevel);
		NamesToValues.Add(AuraRichStyle::FireDmg, FStringFormatArg(FMath::RoundToInt32(Damage)));
		Damage = DamageGameplayAbility->GetDamageAtLevel(AuraGameplayTagsManager::Damage_Lightning, AbilityLevel);
		NamesToValues.Add(AuraRichStyle::LightningDmg, FStringFormatArg(FMath::RoundToInt32(Damage)));
		Damage = DamageGameplayAbility->GetDamageAtLevel(AuraGameplayTagsManager::Damage_Arcane, AbilityLevel);
		NamesToValues.Add(AuraRichStyle::ArcaneDmg, FStringFormatArg(FMath::RoundToInt32(Damage)));
		Damage = DamageGameplayAbility->GetDamageAtLevel(AuraGameplayTagsManager::Damage_Physical, AbilityLevel);
		NamesToValues.Add(AuraRichStyle::PhysicalDmg, FStringFormatArg(FMath::RoundToInt32(Damage)));
	}

	// If the Ability is a projectile spell
	if (const UAuraProjectileSpell* ProjectileSpell = Cast<UAuraProjectileSpell>(AbilityDefault))
	{
		// Get the number of Projectiles
		NamesToValues.Add(AuraRichStyle::ProjectileNum, 
			FStringFormatArg(ProjectileSpell->GetProjectileCount(AbilityLevel)));
	}
	
	// Makes the necessary replacements
	OutText = FString::Format(*OutText, NamesToValues);
}

FAuraAbilityInfo UAuraAbilityInfoDataAsset::FindAuraAbilityInfoForTag(const FGameplayTag& AbilityTag,
                                                                      const bool bLogNotFound) const
{
	// Search in the available abilities if any has the provided ability tag
	for (const FAuraAbilityInfo& AuraAbilityInfo : AbilityInformation)
	{
		if (AuraAbilityInfo.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return AuraAbilityInfo;
		}
	}
	
	// Display logging if not found and marked for logging
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("No Aura Ability Info found in Ability Info Data Asset [%s] for the Ability Tag [%s]"), 
			*GetNameSafe(this) ,*AbilityTag.ToString());
	}
	return FAuraAbilityInfo();
}
