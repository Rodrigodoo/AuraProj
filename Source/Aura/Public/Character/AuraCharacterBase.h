

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/AuraCharacterClassInfoDataAsset.h"
#include "GameFramework/Character.h"
#include "Interaction/AuraCombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UAuraDebuffNiagaraComponent;
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
	// Replication method
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Returns the ability system component to use for this actor.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	// Returns the attribute set to use for this actor.
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	
	// Initializes the character and other components
	// Init the AuraAbilitySystemComponent and stores internal references to AuraAbilitySystemComponent and AttributeSet
	virtual void InitCharacterAndComponents();
	
	virtual void BeginPlay() override;
	
	//~ Begin - IAuraCombatInterface overrides
	
	// Retrieves the Character's class
	virtual EAuraCharacterClass GetCharacterClass_Implementation() const override;
	
	// Returns the location of the combat socket depending on the montage being played
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) const override;
	
	// Return the Hit reaction montage for this character
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	
	// Process character's death (Only on server)
	// Drop Weapon and Ragdoll
	virtual void Die(const FVector& DeathImpulse = FVector::ZeroVector) override;
	
	// Checks if this actor is dead
	virtual bool IsDead_Implementation() const override;
	
	// Apply knockback to the character
	virtual void ApplyKnockback(const FVector& Knockback = FVector::ZeroVector) override;
	
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
	
	// Retrieves the weapon's skeletal mesh (can return null)
	virtual USkeletalMeshComponent* GetWeapon_Implementation() const override;
	
	// Get the number of alive minions bound to this character
	virtual int32 GetMinionCount_Implementation() const override;
	
	// Set the minion count for the alive minions bound to this character
	virtual void SetMinionCount_Implementation(int32 NewMinionCount) override;
	
	// Increment the minion count by a certain amount for the alive minions bound to this character
	// Negative numbers will not be considered
	virtual void IncrementMinionCount_Implementation(int32 Amount = 1) override;
	
	// Decrement the minion count by a certain amount for the alive minions bound to this object
	// Negative numbers will not be considered
	virtual void DecrementMinionCount_Implementation(int32 Amount = 1) override;
	
	// Retrieve the Ability System component delegate for when it is implemented
	virtual FOnAbilitySystemComponentRegistered& GetAbilitySystemComponentRegisteredDelegate() override;
	
	// Retrieve the On Death delegate (Delegate called whenever the character dies)
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	
	// Check if this character is being shocked
	virtual bool IsBeingShocked_Implementation() const override;
	
	// Set if this character is being shocked
	virtual void SetIsBeingShocked_Implementation(const bool InIsBeingShocked) override;
	
	//~ End - IAuraCombatInterface overrides
	
	// Delegate for when the Ability System Component is Registered
	FOnAbilitySystemComponentRegistered OnAbilitySystemComponentRegistered;
	
	// Delegate to broadcast whenever this character dies
	FOnDeathSignature OnDeathDelegate;
	
	// To process on all clients and server when character dies
	// Drop Weapon and Ragdoll
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse = FVector::ZeroVector);

	// Montages to be played while attacking, which socket to use for said attack and what sound to play on impact.
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;

	// Flag to signal this character is stunned
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	bool bIsStunned = false;
	
	// Flag to signal this character is being shocked
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	bool bIsBeingShocked = false;

protected:
	// Skeletal mesh of weapon used by character
	// Note: Not all character may have a weapon so always check validity!
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	// Debuff component to apply burn effects to character whenever the debuff is applied
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UAuraDebuffNiagaraComponent> BurnDebuffComponent;
	
	// Debuff component to apply stun effects to character whenever the debuff is applied
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UAuraDebuffNiagaraComponent> StunDebuffComponent;

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
	
	// This Character's RPG Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Character Class Defaults")
	EAuraCharacterClass CharacterClass = EAuraCharacterClass::Warrior;
	
	// Base walking speed for the character.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 600.f;
	
	// Flag to signal this character has died
	bool bIsDead = false;
	
	// Blood effect to be spawned when taking damage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UNiagaraSystem> BloodEffect;
	
	// Sound to be played when character dies
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USoundBase> DeathSound;

	// Initializes all default attributes for this character
	virtual void InitializeDefaultAttributes() const;
	
	// Applies Gameplay Effect to self
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffect, float Level) const;
	
	// Add abilities to the character
	void AddCharacterAbilities();
	
	// Method to watch for changes in the Stun Tag
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
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

	//~ Begin - Minions
	// Number of currently alive minions bound to this character
	int32 MinionCount = 0;
	
	//~ End - Minions
private:
	// Finds the socket name first in the Character's mesh, then on the weapon if it exists
	// If no socket was found returns the Mesh Component Transform (following GetSocketLocation logic)
	FVector FindSocketLocation(FName SocketName) const;
	
	// Abilities the character should have from the start of the game
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	// Passive abilities the character should have from the start of the game
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
	
	// Hit React montage to play when Character is hit
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
