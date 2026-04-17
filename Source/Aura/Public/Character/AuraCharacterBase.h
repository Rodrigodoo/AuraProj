

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/AuraCombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UNiagaraSystem;
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
	
	virtual void BeginPlay() override;
	
	//~ Begin - IAuraCombatInterface overrides
	
	// Returns the location of the combat socket depending on the montage being played
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) const override;
	
	// Return the Hit reaction montage for this character
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	
	// Process character's death (Only on server)
	// Drop Weapon and Ragdoll
	virtual void Die() override;
	
	// Checks if this actor is dead
	virtual bool IsDead_Implementation() const override;
	
	// Get a reference to this actor
	virtual AActor* GetAvatar_Implementation() override;
	
	// Retrieves all attack montages used by this character
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() const override;
	
	// Finds a random attack montage used by this character
	// Returns true if a valid attack montage was found
	virtual bool FindRandomAttackMontage_Implementation(FTaggedMontage& RandomMontage) const override;
	
	// Finds the attack montage that uses the provided montage tag
	// Returns true if a valid attack montage was found
	virtual bool FindTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag, FTaggedMontage& OutTaggedMontage) const override;
	
	// Retrieves the blood effect for this character
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	//~ End - IAuraCombatInterface overrides
	
	// To process on all clients and server when character dies
	// Drop Weapon and Ragdoll
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	
	// Montages to be played while attacking, which socket to use for said attack and what sound to play on impact.
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;

protected:
	// Skeletal mesh of weapon used by character
	// Note: Not all character may have a weapon so always check validity!
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

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
	virtual void InitializeDefaultAttributes() const;
	
	// Applies Gameplay Effect to self
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffect, float Level) const;
	
	// Add abilities to the character
	void AddCharacterAbilities();
	
	//~ Begin - Dissolve Effects
	
	// Replaces the mesh and weapon materials upon death with their dissolve counterparts
	void Dissolve();
	
	// Starts a Dissolve timeline for the mesh
	UFUNCTION(BlueprintImplementableEvent, Category = "Dissolve Effects")
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	// Starts a Dissolve timeline for the weapon
	UFUNCTION(BlueprintImplementableEvent, Category = "Dissolve Effects")
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	// Dissolve Material Instance to be used on death
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dissolve Effects")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	
	// Weapon Dissolve Material Instance to be used on death
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dissolve Effects")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	
	//~ End - Dissolve Effects
	
	// Flag to signal this character has died
	bool bIsDead = false;
	
	// Blood effect to be spawned when taking damage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UNiagaraSystem> BloodEffect;
private:
	// Finds the socket name first in the Character's mesh, then on the weapon if it exists
	// If no socket was found returns the Mesh Component Transform (following GetSocketLocation logic)
	FVector FindSocketLocation(FName SocketName) const;
	
	// Abilities the character should have from the start of the game
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	// Hit Ract montage to play when Character is hit
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
