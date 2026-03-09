


#include "AuraGameplayTagsManager.h"
#include "GameplayTagsManager.h"

FAuraGameplayTagsManager FAuraGameplayTagsManager::Instance;

void FAuraGameplayTagsManager::InitializeNativeGameplayTags()
{
	/*
	 * Primary Attributes
	 */
	// Attributes.Secondary.Strength
	Instance.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"), 
		FString("Increases physical damage"));
	// Attributes.Primary.Intelligence
	Instance.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"), 
		FString("Increases magical damage, maximum mana, and mana regeneration"));
	// Attributes.Primary.Resilience
	Instance.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"), 
		FString("Increases armor and armor penetration"));
	// Attributes.Primary.Vigor
	Instance.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"), 
		FString("Increases max health and health regeneration"));

	/*
	 * Secondary Attributes
	 */
	// Attributes.Secondary.Armor
	Instance.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), 
		FString("Reduces damage taken, improves block chance"));
	// Attributes.Secondary.ArmorPenetration
	Instance.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"), 
		FString("Ignores enemy's armor, increases critical hit chance"));
	// Attributes.Secondary.BlockChance
	Instance.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"), 
		FString("Chance to cut incoming damage by half"));
	// Attributes.Secondary.CriticalHitChance
	Instance.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"), 
		FString("Chance to double damage plus a critical hit bonus"));
	// Attributes.Secondary.CriticalHitDamage
	Instance.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"), 
		FString("Bonus damage when a critical hit is scored"));
	// Attributes.Secondary.CriticalHitResistance
	Instance.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"), 
		FString("Reduces the critical hit chance of attacking enemies"));
	// Attributes.Secondary.HealthRegeneration
	Instance.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"), 
		FString("Amount of health regenerated every second"));
	// Attributes.Secondary.ManaRegeneration
	Instance.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"), 
		FString("Amount of mana regenerated every second"));
	// Attributes.Secondary.MaxHealth
	Instance.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), 
		FString("Maximum amount of health obtainable"));
	// Attributes.Secondary.MaxMana
	Instance.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"), 
		FString("Maximum amount of mana obtainable"));
}