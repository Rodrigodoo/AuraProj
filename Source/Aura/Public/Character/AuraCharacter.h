

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/AuraPlayerInterface.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;
class AAuraPlayerState;

/**
 * Main character for the game
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IAuraPlayerInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	
	//~ Begin - IAuraCombatInterface
	
	// Get the player's level (the player level will be this character's level)
	virtual int32 GetCharacterLevel_Implementation() const override;
	
	//~ End - IAuraCombatInterface
	
	//~ Begin - IAuraPlayerInterface
	
	// Retrieves the Attribute points reward for said Level
	virtual int32 GetAttributePointsReward_Implementation(const int32 Level) const override;
	
	// Retrieves the Spell points reward for said Level
	virtual int32 GetSpellPointsReward_Implementation(const int32 Level) const override;
	
	// Retrieves the player level for the provided XP 
	virtual int32 FindLevelForXP_Implementation(const int32 XP) const override;
	
	// Retrieves the player's current XP
	virtual int32 GetPlayerXP_Implementation() const override;
	
	// Adds an amount to the Player's XP
	virtual void AddToPlayerXP_Implementation(const int32 XPToAdd) override;
	
	// Gets the player's currently available attribute points
	virtual int32 GetPlayerAttributePoints_Implementation() const override;
	
	// Adds points the player's currently available attribute points
	virtual void AddToPlayerAttributePoints_Implementation(const int32 PointsToAdd) const override;
	
	// Gets the player's currently available spell points
	virtual int32 GetPlayerSpellPoints_Implementation() const override;
	
	// Adds points the player's currently available spell points
	virtual void AddToPlayerSpellPoints_Implementation(const int32 PointsToAdd) const override;
	
	// Levels up the character
	// Only for cosmetics (Player State deals with other level up systems)
	virtual void LevelUp_Implementation() override;
	
	//~ End - IAuraPlayerInterface
	
	//~ Begin - AAuraCharacterBase Overrides
	
	// Initializes the character and other components
	// Init the AuraAbilitySystemComponent and stores internal references to AuraAbilitySystemComponent and AttributeSet
	// initializes the HUD and its Overlay
	virtual void InitCharacterAndComponents() override;
	
	//~ End - AAuraCharacterBase Overrides
	
	//~ Begin - ACharacter Overrides
	
	// Called whenever this character gets possessed by a controller
	virtual void PossessedBy(AController* NewController) override;
	
	//~ End - ACharacter Overrides
	
	//~ Begin - APawn Overrides
	
	// Called when the Player State gets replicated 
	virtual void OnRep_PlayerState() override;
	
	//~ End - APawn Overrides
	
	// Component to display level effects
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
protected:

	// Top down Camera used by this character
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;
	
	// Spring Arm used by the camera
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(EditDefaultsOnly, Category = "Rotation")
	float YawRotationRate = 400.0f;
	
	// Multicast method to spawn level up effects
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
};
