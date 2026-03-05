

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;

/**
 * Aura's Player state in charge of controlling several of the players information like its AbilitySystemComponent
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AAuraPlayerState();
	
	// Replication method override for variable replication
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Returns the ability system component to use for this actor.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	// Returns the attribute set to use for this actor.
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	
	// Returns this player's level
	FORCEINLINE int32 GetPlayerLevel() const { return PlayerLevel; }
	
protected:
	// Pointer to the PlayerState's Ability System Component (If there is one)
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
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
};
