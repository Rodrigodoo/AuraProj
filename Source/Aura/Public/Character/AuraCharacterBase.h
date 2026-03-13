

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/AuraCombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

// Base class for Player and NPCs characters
UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public IAuraCombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	
	// Returns the ability system component to use for this actor.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	// Returns the attribute set to use for this actor.
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	
	// Initializes the character and other components
	// Init the AbilitySystemComponent and stores internal references to AbilitySystemComponent and AttributeSet
	virtual void InitCharacterAndComponents();

protected:
	virtual void BeginPlay() override;
	
	//~ Begin - IAuraCombatInterface overrides
	
	// Returns the location of the weapon's tip
	virtual FVector GetCombatSocketLocation() const override;
	//~ End - IAuraCombatInterface overrides
	
	// Skeletal mesh of weapon used by character
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	// The weapon's tip socket name
	// This will be used to spawn effects or actors like projectiles
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;
	
	// Pointer to the character's Ability System Component (If there is one)
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	// Pointer to the character's Attribute Set (If there is one)
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	// Gameplay effect to define the initial values of the primary attributes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	// Gameplay effect to define the relationship of the secondary attributes with their associated primary attributes
	// NOTE: 
	// - These attributes must be initialized AFTER DefaultPrimaryAttributes
	// - These should be infinite gameplay effects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	
	// Gameplay effect to define the initial values of the vital attributes
	// NOTE: 
	// - These attributes must be initialized AFTER DefaultSecondaryAttributes (They need MaxHealth and MaxMana)
	// - These should be an instant gameplay effects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	// Initializes all default attributes for this character
	void InitializeDefaultAttributes() const;
	
	// Applies Gameplay Effect to self
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffect, float Level) const;
	
	// Add abilities to the character
	void AddCharacterAbilities();
	
private:
	
	// Abilities the character should have from the start of the game
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
