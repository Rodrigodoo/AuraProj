


#include "AuraGameplayTagsManager.h"

namespace AuraGameplayTagsManager
{
	/*
	 * Primary Attributes
	 */
	// Attributes.Secondary.Strength
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Strength, "Attributes.Primary.Strength", 
		"Increases physical damage");
	// Attributes.Primary.Intelligence
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Intelligence, "Attributes.Primary.Intelligence", 
		"Increases magical damage, maximum mana, and mana regeneration");
	// Attributes.Primary.Resilience
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Resilience, "Attributes.Primary.Resilience", 
		"Increases armor and armor penetration");
	// Attributes.Primary.Vigor
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Vigor, "Attributes.Primary.Vigor", 
		"Increases max health and health regeneration");
 
	/*
	 * Secondary Attributes
	 */
	// Attributes.Secondary.Armor
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_Armor, "Attributes.Secondary.Armor", 
		"Reduces damage taken, improves block chance");
	// Attributes.Secondary.ArmorPenetration
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_ArmorPenetration, "Attributes.Secondary.ArmorPenetration", 
		"Ignores enemy's armor, increases critical hit chance");
	// Attributes.Secondary.BlockChance
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_BlockChance, "Attributes.Secondary.BlockChance", 
		"Chance to cut incoming damage by half");
	// Attributes.Secondary.CriticalHitChance
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitChance, "Attributes.Secondary.CriticalHitChance", 
		"Chance to double damage plus a critical hit bonus");
	// Attributes.Secondary.CriticalHitDamage
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitDamage, "Attributes.Secondary.CriticalHitDamage", 
		"Bonus damage when a critical hit is scored");
	// Attributes.Secondary.CriticalHitResistance
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitResistance, "Attributes.Secondary.CriticalHitResistance", 
		"Reduces the critical hit chance of attacking enemies");
	// Attributes.Secondary.HealthRegeneration
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_HealthRegeneration, "Attributes.Secondary.HealthRegeneration", 
		"Amount of health regenerated every second");
	// Attributes.Secondary.ManaRegeneration
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_ManaRegeneration, "Attributes.Secondary.ManaRegeneration", 
		"Amount of mana regenerated every second");
	// Attributes.Secondary.MaxHealth
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_MaxHealth, "Attributes.Secondary.MaxHealth", 
		"Maximum amount of health obtainable");
	// Attributes.Secondary.MaxMana
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_MaxMana, "Attributes.Secondary.MaxMana", 
		"Maximum amount of mana obtainable");
	
	/*
	 * Resistances Attributes
	 */
	// Attributes.Resistance.Fire
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Resistance_Fire, "Attributes.Resistance.Fire", 
		"Resistance to Fire Damage");
	// Attributes.Resistance.Lightning
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Resistance_Lightning, "Attributes.Resistance.Lightning", 
		"Resistance to Lightning Damage");
	// Attributes.Resistance.Arcane
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Resistance_Arcane, "Attributes.Resistance.Arcane", 
		"Resistance to Arcane Damage");
	// Attributes.Resistance.Physical
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Resistance_Physical, "Attributes.Resistance.Physical", 
		"Resistance to Physical Damage");
	
	/*
	 * Abilities
	 */
	// Abilities.Attack
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Attack, "Abilities.Attack", 
		"Attack Ability Tag");
	
	/*
	 * Input
	 */
	// InputTag.LMB
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LMB, "InputTag.LMB", "Input Tag for Left Mouse Button");
	// InputTag.RMB
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_RMB, "InputTag.RMB", "Input Tag for Right Mouse Button");
	// InputTag.1
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_1, "InputTag.1", "Input Tag for 1 key");
	// InputTag.2
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_2, "InputTag.2", "Input Tag for 2 key");
	// InputTag.3
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_3, "InputTag.3", "Input Tag for 3 key");
	// InputTag.4
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_4, "InputTag.4", "Input Tag for 4 key");
	
	/*
	 * Effects
	 */
	// Effects.HitReact
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effects_HitReact, "Effects.HitReact", "Effect to be called upon hit");
	
	/*
	 * Damage & Damage Types
	 */
	// Damage
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage, "Damage", "Damage");
	// Damage.Fire
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Fire, "Damage.Fire", "Fire Damage Type");
	// Damage.Lightning
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Lightning, "Damage.Lightning", "Lightning Damage Type");
	// Damage.Arcane
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Arcane, "Damage.Arcane", "Arcane Damage Type");
	// Damage.Physical
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Physical, "Damage.Physical", "Physical Damage Type");
	
	/*
	 * Montages
	 */
	// Montage.Attack.Weapon
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_Weapon, "Montage.Attack.Weapon", "Weapon Attack Montage");
	// Montage.Attack.RightHand
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_RightHand, "Montage.Attack.RightHand", "Right Hand Attack Montage");
	// Montage.Attack.LeftHand
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_LeftHand, "Montage.Attack.LeftHand", "Left Hand Attack Montage");
	
	/*
	 * Mapping of Damage Types to Resistances
	 */
	const TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances = 
	{
		// Fire Damage - Fire Resistance
		{Damage_Fire, Attributes_Resistance_Fire},
		// Lightning Damage - Lightning Resistance
		{Damage_Lightning, Attributes_Resistance_Lightning},
		// Arcane Damage - Arcane Resistance
		{Damage_Arcane, Attributes_Resistance_Arcane},
		// Physical Damage - Physical Resistance
		{Damage_Physical, Attributes_Resistance_Physical}
	};
}