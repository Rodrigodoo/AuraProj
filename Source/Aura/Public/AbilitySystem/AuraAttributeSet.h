

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
	 * Begin - Vital  Attributes
	 */
	
	//~ Character's Health
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Health); // Generates All basic accessor functions
	
	//~ Character's Max Health
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, MaxHealth); // Generates All basic accessor functions
	
	// Replication method for Health
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData & OldHealth) const;
	
	// Replication method for Max Health
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData & OldMaxHealth) const;

	//~ Character's Mana
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Mana); // Generates All basic accessor functions
	
	//~ Character's Max Mana
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, MaxMana); // Generates All basic accessor functions
	
	// Replication method for Mana
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData & OldMana) const;
	
	// Replication method for Max Mana
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData & OldMaxMana) const;
	
	/*
	 * End - Vital  Attributes
	 */
	
	
	/*
	 * Begin - Primary Attributes
	 */
	
	//~ Character's Strength
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Strength); // Generates All basic accessor functions
	
	// Replication method for Strength
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData & OldStrength) const;
	
	//~ Character's Intelligence
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Intelligence); // Generates All basic accessor functions
	
	// Replication method for Intelligence
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData & OldIntelligence) const;
	
	//~ Character's Resilience
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Resilience, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Resilience); // Generates All basic accessor functions
	
	// Replication method for Resilience
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData & OldResilience) const;
	
	//~ Character's Vigor
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Vigor, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Vigor); // Generates All basic accessor functions
	
	// Replication method for Vigor
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData & OldVigor) const;
	
	/*
	 * End - Primary Attributes
	 */


private:
	
	// Fill in the Effect Properties for this effect
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& OutEffectProperties) const;
};
