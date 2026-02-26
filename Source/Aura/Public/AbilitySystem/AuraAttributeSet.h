

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

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
	
	//~ Begin - Vital  Attributes
	
	// Character's Health
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Health); // Generates All basic accessor functions
	
	// Character's Max Health
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, MaxHealth); // Generates All basic accessor functions
	
	// Replication method for Health
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData & OldHealth) const;
	
	// Replication method for Max Health
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData & OldMaxHealth) const;
	//~ End - Vital  Attributes
	
	// ~ Begin Consumable Attributes
	
	// Character's Mana
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Consumable Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Mana); // Generates All basic accessor functions
	
	// Character's Max Mana
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Consumable Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, MaxMana); // Generates All basic accessor functions
	
	// Replication method for Mana
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData & OldMana) const;
	
	// Replication method for Max Mana
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData & OldMaxMana) const;
	// ~ End Consumable Attributes
};
