

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/AuraPlayerInterface.h"
#include "AuraCharacter.generated.h"

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
	virtual int32 GetCharacterLevel() const override;
	
	//~ End - IAuraCombatInterface
	
	//~ Begin - IAuraPlayerInterface
	
	// Adds an amount to the Player's XP
	virtual void AddToPlayerXP_Implementation(const int32 XPToAdd) override;
	
	//~ End - IAuraPlayerInterface
	
	//~ Begin - AAuraCharacterBase Overrides
	
	// Initializes the character and other components
	// Init the AbilitySystemComponent and stores internal references to AbilitySystemComponent and AttributeSet
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
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Rotation")
	float YawRotationRate = 400.0f;
};
