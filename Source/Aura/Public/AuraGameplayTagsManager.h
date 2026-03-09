

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Contains and manages all the Aura project's Gameplay Tags (Singleton)
 */
struct FAuraGameplayTagsManager
{
public:
	// Returns the Instance of this manager
	const static FAuraGameplayTagsManager& Get() { return Instance; };
	
	// Initialises the Gameplay Tags for this project
	// Enrolls them in the UGameplayTagManager (Unreal Version)
	static void InitializeNativeGameplayTags();
	
	/*
	 * Begin - Internal references to Gameplay Tags
	 */
	
	//~ Begin - Internal references to Gameplay Tags
	
	//~ Begin - Attributes
	//~ Begin - Attributes Primary
	// Attributes.Primary.Strength
	FGameplayTag Attributes_Primary_Strength;
	// Attributes.Primary.Intelligence
	FGameplayTag Attributes_Primary_Intelligence;
	// Attributes.Primary.Resilience
	FGameplayTag Attributes_Primary_Resilience;
	// Attributes.Primary.Vigor
	FGameplayTag Attributes_Primary_Vigor;
	//~ End - Attributes Primary
	
	//~ Begin - Attributes Secondary
	// Attributes.Secondary.Armor
	FGameplayTag Attributes_Secondary_Armor;
	// Attributes.Secondary.ArmorPenetration
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	// Attributes.Secondary.BlockChance
	FGameplayTag Attributes_Secondary_BlockChance;
	// Attributes.Secondary.CriticalHitChance
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	// Attributes.Secondary.CriticalHitDamage
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	// Attributes.Secondary.CriticalHitResistance
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	// Attributes.Secondary.HealthRegeneration
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	// Attributes.Secondary.ManaRegeneration
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	// Attributes.Secondary.MaxHealth
	FGameplayTag Attributes_Secondary_MaxHealth;
	// Attributes.Secondary.MaxMana
	FGameplayTag Attributes_Secondary_MaxMana;
	//~ End - Attributes Secondary
	//~ End - Attributes
	/*
	 * End - Internal references to Gameplay Tags
	 */
protected:
	
private:
	// This Manager's Instance
	static FAuraGameplayTagsManager Instance;
	
};