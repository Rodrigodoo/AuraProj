

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
	//~ End - Attributes
	
	//~ Begin - Abilities
	// Abilities.Attack
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Attack);
	//~ End - Abilities
	
	//~ Begin - Inputs
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
	
	// Stores all Damage Types tags and relates them to resistances
	extern const TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
}