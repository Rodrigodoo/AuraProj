

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

class AAuraPlayerState;

/**
 * Main character for the game
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	
	// Initializes the AbilitySystemComponent and stores internal references to AbilitySystemComponent and AttributeSet
	void InitAndCacheAbilitySystemComponentAndAttributeSet(AAuraPlayerState* AuraPlayerState);
	
	// Called whenever this character gets possessed by a controller
	virtual void PossessedBy(AController* NewController) override;
	
	// Called when the Player State gets replicated 
	virtual void OnRep_PlayerState() override;
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Rotation")
	float YawRotationRate = 400.0f;
};
