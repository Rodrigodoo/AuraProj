

#pragma once

#include "CoreMinimal.h"
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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	//~ Begin - Vital  Attributes
	
	// Character's Health
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	
	// Character's Max Health
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	
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
	
	// Character's Max Mana
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Consumable Attributes")
	FGameplayAttributeData MaxMana;
	
	// Replication method for Mana
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData & OldMana) const;
	
	// Replication method for Max Mana
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData & OldMaxMana) const;
	// ~ End Consumable Attributes
};
