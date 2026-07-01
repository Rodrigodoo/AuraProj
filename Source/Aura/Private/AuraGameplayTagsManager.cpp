


#include "AuraGameplayTagsManager.h"

namespace AuraGameplayTagsManager
{
	namespace DebuffIndex
	{
		uint8 Chance = 0;
		uint8 Damage = 1;
		uint8 Frequency = 2;
		uint8 Duration = 3;
		uint8 ShouldHitReact = 4;
	}
	
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
	 * Meta Attributes
	 */
	// Attributes.Meta
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Meta, "Attributes.Meta", "Meta Attributes");
	// Attributes.Meta.IncomingXP
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Meta_IncomingXP, "Attributes.Meta.IncomingXP", 
		"Incoming XP Meta Attribute");
	
	/*
	 * Abilities
	 */
	// Abilities
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities, "Abilities", 
		"Ability Tag");
	// Abilities.Attack
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Attack, "Abilities.Attack", 
		"Attack Ability Tag");
	// Abilities.Summon
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Summon, "Abilities.Summon", 
		"Summon Ability Tag");
	// Abilities.Fire
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Fire, "Abilities.Fire", "Fire Ability type Tag");
	// Abilities.Fire.FireBolt
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Fire_FireBolt, "Abilities.Fire.FireBolt", 
		"FireBolt Ability Tag");
	// Abilities.Lightning
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Lightning, "Abilities.Lightning", "Lightning Ability type Tag");
	// Abilities.Lightning.Electrocute
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Lightning_Electrocute, "Abilities.Lightning.Electrocute",
		"Electrocute Ability Tag");
	// Abilities.HitReact
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_HitReact, "Abilities.HitReact", 
		"Hit React Ability Tag");
	
	/*
	 * Abilities Status
	 */
	// Abilities.Status
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Status, "Abilities.Status", 
		"Ability Status Tag");
	// Abilities.Status.Locked
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Status_Locked, "Abilities.Status.Locked", 
		"Ability Status Locked Tag");
	// Abilities.Status.Eligible
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Status_Eligible, "Abilities.Status.Eligible", 
		"Ability Status Eligible Tag");
	// Abilities.Status.Unlocked
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Status_Unlocked, "Abilities.Status.Unlocked", 
		"Ability Status Unlocked Tag");
	// Abilities.Status.Equipped
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Status_Equipped, "Abilities.Status.Equipped", 
		"Ability Status Equipped Tag");
	
	/*
	 * Abilities Type
	 */
	// Abilities.Type
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Type, "Abilities.Type", 
		"Ability Type Tag");
	// Abilities.Type.Offensive
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Type_Offensive, "Abilities.Type.Offensive", 
		"Ability Type Offensive Tag");
	// Abilities.Type.Passive
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Type_Passive, "Abilities.Type.Passive", 
		"Ability Type Passive Tag");
	// Abilities.Type.None
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Type_None, "Abilities.Type.None", 
		"Ability Type None Tag");
	
	/*
	 * Cooldowns
	 */
	// Abilities
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooldown, "Cooldown", 
		"Cooldown Tag");
	// Cooldown.Fire.FireBolt
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooldown_Fire_FireBolt, "Cooldown.Fire.FireBolt", 
		"Firebolt Cooldown Tag");
	
	/*
	 * Input
	 */
	// InputTag.LMB
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag, "InputTag", "Input Tag");
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
	// InputTag.Passive.1
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Passive1, "InputTag.Passive.1", "Input Tag for Passive 1 key");
	// InputTag.Passive.2
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Passive2, "InputTag.Passive.2", "Input Tag for Passive 2 key");
	
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
	 * Debuffs
	 */
	// Debuff
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff, "Debuff", "Debuff");
	// Debuff.Burn
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Burn, "Debuff.Burn", "Burn Debuff Caused by Fire Damage");
	// Debuff.Burn.Chance
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Burn_Chance, "Debuff.Burn.Chance", "Chance to Cause the Burn Debuff");
	// Debuff.Burn.Damage
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Burn_Damage, "Debuff.Burn.Damage", "Damage Caused by the Burn Debuff");
	// Debuff.Burn.Frequency
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Burn_Frequency, "Debuff.Burn.Frequency", "Frequency of the Burn Debuff");
	// Debuff.Burn.Duration
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Burn_Duration, "Debuff.Burn.Duration", "Duration of the Burn Debuff");
	// Debuff.Burn.ShouldHitReact
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Burn_ShouldHitReact, "Debuff.Burn.ShouldHitReact", "Should Burn Hit React");
	// Debuff.Stun
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Stun, "Debuff.Stun", "Stun Debuff Caused by Lightning Damage");
	// Debuff.Stun.Chance
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Stun_Chance, "Debuff.Stun.Chance", "Chance to Cause the Stun Debuff");
	// Debuff.Stun.Damage
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Stun_Damage, "Debuff.Stun.Damage", "Damage Caused by the Stun Debuff");
	// Debuff.Stun.Frequency
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Stun_Frequency, "Debuff.Stun.Frequency", "Frequency of the Stun Debuff");
	// Debuff.Stun.Duration
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Stun_Duration, "Debuff.Stun.Duration", "Duration of the Stun Debuff");
	// Debuff.Stun.ShouldHitReact
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Stun_ShouldHitReact, "Debuff.Stun.ShouldHitReact", "Should Stun Hit React");
	// Debuff.Arcane
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Arcane, "Debuff.Arcane", "Arcane Debuff Caused by Arcane Damage");
	// Debuff.Arcane.Chance
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Arcane_Chance, "Debuff.Arcane.Chance", "Chance to Cause the Arcane Debuff");
	// Debuff.Arcane.Damage
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Arcane_Damage, "Debuff.Arcane.Damage", "Damage Caused by the Arcane Debuff");
	// Debuff.Arcane.Frequency
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Arcane_Frequency, "Debuff.Arcane.Frequency", "Frequency of the Arcane Debuff");
	// Debuff.Arcane.Duration
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Arcane_Duration, "Debuff.Arcane.Duration", "Duration of the Arcane Debuff");
	// Debuff.Arcane.ShouldHitReact
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Arcane_ShouldHitReact, "Debuff.Arcane.ShouldHitReact", "Should Arcane Hit React");
	// Debuff.Physical
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Physical, "Debuff.Physical", "Physical Debuff Caused by Physical Damage");
	// Debuff.Physical.Chance
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Physical_Chance, "Debuff.Physical.Chance", "Chance to Cause the Physical Debuff");
	// Debuff.Physical.Damage
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Physical_Damage, "Debuff.Physical.Damage", "Damage Caused by the Physical Debuff");
	// Debuff.Physical.Frequency
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Physical_Frequency, "Debuff.Physical.Frequency", "Frequency of the Physical Debuff");
	// Debuff.Physical.Duration
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Physical_Duration, "Debuff.Physical.Duration", "Duration of the Physical Debuff");
	// Debuff.Physical.ShouldHitReact
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Physical_ShouldHitReact, "Debuff.Physical.ShouldHitReact", "Should Physical Hit React");
	
	/*
	 * Montages
	 */
	// Montage.Attack.Weapon
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_Weapon, "Montage.Attack.Weapon", "Weapon Attack Montage");
	// Montage.Attack.RightHand
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_RightHand, "Montage.Attack.RightHand", "Right Hand Attack Montage");
	// Montage.Attack.LeftHand
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_LeftHand, "Montage.Attack.LeftHand", "Left Hand Attack Montage");
	// Montage.Attack.RightTailSwipe
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_RightTailSwipe, "Montage.Attack.RightTailSwipe", "Right Tail Attack Montage");
	// Montage.Attack.LeftTailSwipe
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_LeftTailSwipe, "Montage.Attack.LeftTailSwipe", "Left Tail Attack Montage");
	
	/*
	 * Player
	 */
	// Player
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player, "Player", "Parent Player Tag");
	// Player.Block.InputPressed
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Block_InputPressed, "Player.Block.InputPressed", "Block Input Pressed Callback");
	// Player.Block.InputHeld
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Block_InputHeld, "Player.Block.InputHeld", "Block Input Held Callback");
	// Player.Block.InputReleased
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Block_InputReleased, "Player.Block.InputReleased", "Block Input Released Callback");
	// Player.Block.CursorTrace
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Block_CursorTrace, "Player.Block.CursorTrace", "Block Tracing under the cursor");

	
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
	
	/*
	 * Mapping of Damage Types to Debuffs
	 */
	const TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs = 
	{
		// Fire Damage - Burn Debuffs
		{Damage_Fire, Debuff_Burn},
		// Lightning Damage - Stun Debuffs
		{Damage_Lightning, Debuff_Stun},
		// Arcane Damage - Arcane Debuffs
		{Damage_Arcane, Debuff_Arcane},
		// Physical Damage - Physical Debuffs
		{Damage_Physical, Debuff_Physical}
	};

	/*
	 * Mapping of Debuff Types to Stats
	 */
	const TMap<FGameplayTag, TArray<FGameplayTag>> DebuffTypeToStats
	{
		// Burn Debuff - Debuff Stats
		{Debuff_Burn, 
			{Debuff_Burn_Chance, Debuff_Burn_Damage, Debuff_Burn_Frequency, 
				Debuff_Burn_Duration, Debuff_Burn_ShouldHitReact}},
		// Stun Debuff - Debuff Stats
		{Debuff_Stun, 
			{Debuff_Stun_Chance, Debuff_Stun_Damage, Debuff_Stun_Frequency, 
				Debuff_Stun_Duration, Debuff_Stun_ShouldHitReact}},
		// Arcane Debuff - Debuff Stats
		{Debuff_Arcane, 
			{Debuff_Arcane_Chance, Debuff_Arcane_Damage, Debuff_Arcane_Frequency, 
				Debuff_Arcane_Duration, Debuff_Arcane_ShouldHitReact}},
		// Physical Debuff - Debuff Stats
		{Debuff_Physical, 
			{Debuff_Physical_Chance, Debuff_Physical_Damage, Debuff_Physical_Frequency, 
				Debuff_Physical_Duration, Debuff_Physical_ShouldHitReact}}
	};

	const FGameplayTag& GetDebuffChanceByType(const FGameplayTag& DebuffTag)
	{
		return DebuffTypeToStats[DebuffTag][DebuffIndex::Chance];
	}

	const FGameplayTag& GetDebuffDamageByType(const FGameplayTag& DebuffTag)
	{
		return DebuffTypeToStats[DebuffTag][DebuffIndex::Damage];
	}

	const FGameplayTag& GetDebuffFrequencyByType(const FGameplayTag& DebuffTag)
	{
		return DebuffTypeToStats[DebuffTag][DebuffIndex::Frequency];
	}

	const FGameplayTag& GetDebuffDurationByType(const FGameplayTag& DebuffTag)
	{
		return DebuffTypeToStats[DebuffTag][DebuffIndex::Duration];
	}

	const FGameplayTag& GetDebuffShouldHitReactByType(const FGameplayTag& DebuffTag)
	{
		return DebuffTypeToStats[DebuffTag][DebuffIndex::ShouldHitReact];
	}
}
