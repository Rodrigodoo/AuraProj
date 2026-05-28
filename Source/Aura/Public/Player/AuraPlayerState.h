

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Interaction/AuraPlayerInterface.h"
#include "AuraPlayerState.generated.h"


class UAuraAbilitySystemComponent;
class UAuraLevelUpInfoDataAsset;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32 /*StatValue*/)

/**
 * Aura's Player state in charge of controlling several of the players information like its AuraAbilitySystemComponent
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AAuraPlayerState();
	
	// Replication method override for variable replication
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	//~ Begin - IAbilitySystemInterface
	
	// Returns the ability system component to use for this actor.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	//~ End - IAbilitySystemInterface
	
	// Returns the aura ability system component to use for this actor.
	UAbilitySystemComponent* GetAuraAbilitySystemComponent() const;
	
	// Returns the attribute set to use for this actor.
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	
	// Set the player Level
	void SetPlayerLevel(int32 NewPlayerLevel);
	
	// Adds an amount to the player's Level
	void AddToPlayerLevel(int32 LevelsToAdd);
	
	// Returns this player's level
	FORCEINLINE int32 GetPlayerLevel() const { return PlayerLevel; }
	
	// Set the player XP
	void SetPlayerXP(const int32 NewXP);
	
	// Adds an amount to the Player's XP
	void AddToPlayerXP(const int32 XPToAdd);
	
	// Finds the player's level for the provided XP
	int32 FindLevelForXP(const int32 XP) const;
	
	// Returns the player XP
	FORCEINLINE int32 GetPlayerXP() const { return PlayerXP; }
	
	// Returns the currently unused attribute points
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }

	// Sets the currently unused attribute points
	void SetAttributePoints(const int32 NewAttributePoints);
	
	// Adds points to the currently unused attribute points
	void AddToAttributePoints(const int32 AttributePointsToAdd);

	// Returns the currently unused spell points
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }

	// Sets the currently unused spell points
	void SetSpellPoints(const int32 NewSpellPoints);
	
	// Adds points to the currently unused spell points
	void AddToSpellPoints(const int32 SpellPointsToAdd);

	// Finds the Attribute points reward for said level
	int32 FindAttributePointRewardForLevel(const int32 Level) const;
	
	// Finds the Spell points reward for said level
	int32 FindSpellPointRewardForLevel(const int32 Level) const;
	
	// Data Asset with Level Up Information
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level Up Data")
	TObjectPtr<UAuraLevelUpInfoDataAsset> LevelUpInfoDataAsset;
	
	//~ Begin - Delegates
	
	// Delegate to signal whenever the player's XP changed
	FOnPlayerStatChangedSignature OnPlayerXPChangedDelegate;
	
	// Delegate to signal whenever the player's Level changed
	FOnPlayerStatChangedSignature OnPlayerLevelChangedDelegate;
	
	// Delegate to signal whenever the player's Attribute Points changed
	FOnPlayerStatChangedSignature OnPlayerAttributePointsChangedDelegate;
	
	// Delegate to signal whenever the player's Spell Points changed
	FOnPlayerStatChangedSignature OnPlayerSpellPointsChangedDelegate;
	
	//~ End - Delegates
	
protected:
	// Pointer to the PlayerState's Ability System Component (If there is one)
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	
	// Pointer to the PlayerState's Attribute Set (If there is one)
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	// The Player's Level
	// This value works as a counter of the player's progress
	// It will influence attribute calculations
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_PlayerLevel)
	int32 PlayerLevel = 1;
	
	// Replication method for Level
	UFUNCTION()
	void OnRep_PlayerLevel(int32 OldPlayerLevel);
	
	// Current Experience Points for this player (cumulative)
	// This tracks the player's progression and can be translated into the Player Level
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_PlayerXP)
	int32 PlayerXP;
	
	// Replication method for the player PlayerXP
	UFUNCTION()
	void OnRep_PlayerXP(int32 OldPlayerXP);
	
	// Currently unused attribute points
	// These are used to increase attribute values
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 0;
	
	// Replication method for AttributePoints
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);
	
	// Currently unused spell point
	// Used to purchase new spells and increase spell levels
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
	int32 SpellPoints = 0;
	
	// Replication method for Spell Points
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
};
