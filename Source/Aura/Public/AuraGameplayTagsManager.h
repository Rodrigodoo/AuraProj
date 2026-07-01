

#pragma once

#include "NativeGameplayTags.h"

// Tag used to signal an actor is a Player
#define ACTOR_PlAYER_TAG "Player" 

// Tag used to signal an actor is an Enemy
#define ACTOR_ENEMY_TAG "Enemy"

/**
 * Contains and manages all the Aura project's Gameplay Tags
 */
namespace AuraGameplayTagsManager
{
	//~ Begin - Attributes
	//~ Begin - Attributes Primary
	// Attributes.Primary.Strength
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Strength);
	// Attributes.Primary.Intelligence
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Intelligence);
	// Attributes.Primary.Resilience
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Resilience);
	// Attributes.Primary.Vigor
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Vigor);
	//~ End - Attributes Primary
	
	//~ Begin - Attributes Secondary
	// Attributes.Secondary.Armor
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_Armor);
	// Attributes.Secondary.ArmorPenetration
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ArmorPenetration);
	// Attributes.Secondary.BlockChance
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_BlockChance);
	// Attributes.Secondary.CriticalHitChance
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitChance);
	// Attributes.Secondary.CriticalHitDamage
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitDamage);
	// Attributes.Secondary.CriticalHitResistance
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitResistance);
	// Attributes.Secondary.HealthRegeneration
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_HealthRegeneration);
	// Attributes.Secondary.ManaRegeneration
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ManaRegeneration);
	// Attributes.Secondary.MaxHealth
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxHealth);
	// Attributes.Secondary.MaxMana
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxMana);
	//~ End - Attributes Secondary
	
	//~ Begin - Attributes Resistance
	// Attributes.Resistance.Fire
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Fire);
	// Attributes.Resistance.Lightning
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Lightning);
	// Attributes.Resistance.Arcane
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Arcane);
	// Attributes.Resistance.Physical
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Physical);
	//~ End - Attributes Resistances
	
	//~ Begin - Meta Attributes
	// Attributes.Meta
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Meta);
	// Attributes.Meta.IncomingXP
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Meta_IncomingXP);
	//~ End - Meta Attributes
	
	//~ End - Attributes
	
	//~ Begin - Abilities
	// Abilities
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities);
	// Abilities.Attack
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Attack);
	// Abilities.Summon
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Summon);
	// Abilities.Fire
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Fire);
	// Abilities.Fire.FireBolt
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Fire_FireBolt);
	// Abilities.Lightning
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Lightning);
	// Abilities.Lightning.Electrocute
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Lightning_Electrocute);
	// Abilities.HitReact
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_HitReact);
	
	//~ Begin - Abilities Status
	// Abilities.Status
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status);
	// Abilities.Status.Locked
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Locked);
	// Abilities.Status.Eligible
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Eligible);
	// Abilities.Status.Unlocked
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Unlocked);
	// Abilities.Status.Equipped
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Equipped);
	//~ End - Abilities Status
	
	//~ Begin - Abilities Type
	// Abilities.Type
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Type);
	// Abilities.Type.Offensive
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Type_Offensive);
	// Abilities.Type.Passive
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Type_Passive);
	// Abilities.Type.None
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Type_None);
	//~ End - Abilities Type
	
	//~ End - Abilities
	
	//~ Begin - Cooldowns
	// Cooldown
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown);
	// Cooldown.Fire.FireBolt
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_Fire_FireBolt);
	//~ End - Cooldowns
	
	//~ Begin - Inputs
	// InputTag
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag);
	// InputTag.LMB
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LMB);
	// InputTag.RMB
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RMB);
	// InputTag.1
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_1);
	// InputTag.2
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_2);
	// InputTag.3
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_3);
	// InputTag.4
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_4);
	// InputTag.Passive.1
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Passive_1);
	// InputTag.Passive.2
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Passive_2);
	//~ End - Inputs
	
	//~ Begin - Effects
	// Effects.HitReact
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effects_HitReact);
	//~ End - Effects
	
	//~ Begin - Damage & Damage Types
	// Damage
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage);
	// Damage.Fire
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Fire);
	// Damage.Lightning
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Lightning);
	// Damage.Arcane
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Arcane);
	// Damage.Physical
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Physical);
	//~ End - Damage & Damage Types
	
	//~ Begin - Debuffs
	// Debuff
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff);
	// Debuff.Burn
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Burn);
	// Debuff.Burn.Chance
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Burn_Chance);
	// Debuff.Burn.Damage
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Burn_Damage);
	// Debuff.Burn.Frequency
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Burn_Frequency);
	// Debuff.Burn.Duration
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Burn_Duration);
	// Debuff.Burn.ShouldHitReact
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Burn_ShouldHitReact);
	
	// Debuff.Burn.Stun
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Stun);
	// Debuff.Stun.Chance
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Stun_Chance);
	// Debuff.Stun.Damage
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Stun_Damage);
	// Debuff.Stun.Frequency
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Stun_Frequency);
	// Debuff.Stun.Duration
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Stun_Duration);
	// Debuff.Stun.ShouldHitReact
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Stun_ShouldHitReact);
	
	// Debuff.Arcane
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Arcane);
	// Debuff.Arcane.Chance
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Arcane_Chance);
	// Debuff.Arcane.Damage
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Arcane_Damage);
	// Debuff.Arcane.Frequency
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Arcane_Frequency);
	// Debuff.Arcane.Duration
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Arcane_Duration);
	// Debuff.Arcane.ShouldHitReact
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Arcane_ShouldHitReact);
	
	// Debuff.Physical
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Physical);
	// Debuff.Physical.Chance
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Physical_Chance);
	// Debuff.Physical.Damage
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Physical_Damage);
	// Debuff.Physical.Frequency
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Physical_Frequency);
	// Debuff.Physical.Duration
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Physical_Duration);
	// Debuff.Physical.ShouldHitReact
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Physical_ShouldHitReact);
	//~ End - Debuffs
	
	//~ Begin - Montages
	// Montage.Attack.Weapon
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_Weapon);
	// Montage.Attack.RightHand
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_RightHand);
	// Montage.Attack.LeftHand
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_LeftHand);
	// Montage.Attack.RightTailSwipe
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_RightTailSwipe);
	// Montage.Attack.LeftTailSwipe
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_LeftTailSwipe);
	//~ End - Montages
	
	//~ Begin - Player
	// Player
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player);
	// Player.Block.InputPressed
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Block_InputPressed);
	// Player.Block.InputHeld
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Block_InputHeld);
	// Player.Block.InputReleased
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Block_InputReleased);
	// Player.Block.CursorTrace
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Block_CursorTrace);
	//~ End - Player
	
	// Stores all Damage Types tags and relates them to resistances
	extern const TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	
	// Stores all Damage Types tags and relates them to Debuffs
	extern const TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;
	
	// Stores all Debuff Type tags and relates them to their stats
	extern const TMap<FGameplayTag, TArray<FGameplayTag>> DebuffTypeToStats;
	
	// Retrieve the Debuff Chance for this specific Debuff
	const FGameplayTag& GetDebuffChanceByType(const FGameplayTag& DebuffTag);
	// Retrieve the Debuff Damage for this specific Debuff
	const FGameplayTag& GetDebuffDamageByType(const FGameplayTag& DebuffTag);
	// Retrieve the Debuff Frequency for this specific Debuff
	const FGameplayTag& GetDebuffFrequencyByType(const FGameplayTag& DebuffTag);
	// Retrieve the Debuff Duration for this specific Debuff
	const FGameplayTag& GetDebuffDurationByType(const FGameplayTag& DebuffTag);
	// Retrieve the Debuff ShouldHitReact for this specific Debuff
	const FGameplayTag& GetDebuffShouldHitReactByType(const FGameplayTag& DebuffTag);
}