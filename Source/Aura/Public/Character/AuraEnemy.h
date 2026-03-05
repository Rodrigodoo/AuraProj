

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/AuraEnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IAuraEnemyInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();

	//~ Begin - IAuraEnemyInterface
	// Highlights the enemy
	virtual void HighlightActor() override;
	
	// Unhighlights the enemy
	virtual void UnHighlightActor() override;
	//~ End - IAuraEnemyInterface
	
	//~ Begin - IAuraCombatInterface
	// Get this enemy's level
	virtual int32 GetCharacterLevel() override;
	//~ End - IAuraCombatInterface
	
	// Initializes the character and other components
	// Init the AbilitySystemComponent and stores internal references to AbilitySystemComponent and AttributeSet
	virtual void InitCharacterAndComponents() override;
protected:
	virtual void BeginPlay() override;
	
	// The Enemy's Level
	// This value works as an identifier of the enemy's difficulty
	// It will influence its attribute calculations
	// Note: Note replicated because only the server needs to know this information
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Character Class Defaults")
	int32  CharacterLevel = 1;
};
