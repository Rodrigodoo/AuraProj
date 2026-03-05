

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

/**
 * Struct to store important information about a Gameplay effect
 */
USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()
	
	FEffectProperties(){};
	
	// Context Handle for this effect
	FGameplayEffectContextHandle EffectContextHandle;
	
	//~ Begin Source
	// Source Ability System Component for this effect
	UPROPERTY()
	UAbilitySystemComponent* SourceAbilitySystemComponent = nullptr;
	
	// Source Avatar actor for this effect
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	
	// Source Controller for this effect
	UPROPERTY()
	AController* SourceController = nullptr;
	
	// Source Character for this effect
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;
	//~ End Source
	
	//~ Begin Target
	// Target Ability System Component for this effect
	UPROPERTY()
	UAbilitySystemComponent* TargetAbilitySystemComponent = nullptr;
	
	// Target Avatar actor for this effect
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	
	// Target Controller for this effect
	UPROPERTY()
	AController* TargetController = nullptr;
	
	// Target Character for this effect
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
	//~ End Target
};

/**
 * Attribute Set in charge of holding attributes for the Aura project
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAuraAttributeSet();
	// Replication method override for variable replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Called before an attribute change
	// should only be used for clamping
	// Later modifications might affect this value even after clamping it
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	// Called after a Gameplay Effect is exectuded
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	/*
	 * Begin - Primary Attributes
	 */
	
	//~ Character's Strength
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Strength); // Generates All basic accessor functions
	
	//~ Character's Intelligence
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Intelligence); // Generates All basic accessor functions

	//~ Character's Resilience
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Resilience, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Resilience); // Generates All basic accessor functions

	//~ Character's Vigor
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Vigor, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Vigor); // Generates All basic accessor functions
	
	// Replication method for Strength
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData & OldStrength) const;
	
	// Replication method for Intelligence
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData & OldIntelligence) const;
	
	// Replication method for Resilience
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData & OldResilience) const;
	
	// Replication method for Vigor
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData & OldVigor) const;
	
	/*
	 * End - Primary Attributes
	 */
	
	/*
	 * Begin - Secondary Attributes
	 */
	
	//~ Character's Armor (depends on Resilience)
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Armor); // Generates All basic accessor functions
	
	//~ Character's Armor Penetration (depends on Resilience)
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, ArmorPenetration); // Generates All basic accessor functions
	
	//~ Character's Block Chance (depends on Armor)
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, BlockChance); // Generates All basic accessor functions
	
	//~ Character's Critical Hit Chance (depends on Armor Penetration)
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, CriticalHitChance); // Generates All basic accessor functions
	
	//~ Character's Critical Hit Damage (depends on Armor Penetration)
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, CriticalHitDamage); // Generates All basic accessor functions
	
	//~ Character's Critical Hit Resistance (depends on Armor)
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, CriticalHitResistance); // Generates All basic accessor functions
	
	//~ Character's Health Regeneration (depends on Vigor)
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, HealthRegeneration); // Generates All basic accessor functions
	
	//~ Character's Mana Regeneration (depends on Intelligence)
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, ManaRegeneration); // Generates All basic accessor functions

	//~ Character's Max Health (depends on Vigor)
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, MaxHealth); // Generates All basic accessor functions
	
	//~ Character's Max Mana (depends on Intelligence)
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, MaxMana); // Generates All basic accessor functions
	
	// Replication method for Armor
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;
	
	// Replication method for ArmorPenetration
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;
	
	// Replication method for BlockChance
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
	
	// Replication method for CriticalHitChance
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;
	
	// Replication method for CriticalHitDamage
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;
	
	// Replication method for CriticalHitResistance
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;
	
	// Replication method for HealthRegeneration
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;
	
	// Replication method for ManaRegeneration
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;
	
	// Replication method for Max Health
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData & OldMaxHealth) const;
	
	// Replication method for Max Mana
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData & OldMaxMana) const;
	/*
	 * End - Secondary Attributes
	 */
	
	/*
	 * Begin - Vital  Attributes
	 */
	
	//~ Character's Health
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Health); // Generates All basic accessor functions
	
	// Replication method for Health
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData & OldHealth) const;

	//~ Character's Mana
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Mana); // Generates All basic accessor functions

	// Replication method for Mana
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData & OldMana) const;

	/*
	 * End - Vital  Attributes
	 */

private:
	
	// Fill in the Effect Properties for this effect
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& OutEffectProperties) const;
};
