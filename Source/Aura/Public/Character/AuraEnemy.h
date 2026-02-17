

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	//---IEnemyInterface---Start
	
	// Highlights the enemy
	virtual void HighlightActor();
	
	// Unhighlights the enemy
	virtual void UnHighlightActor();
	
	//---IEnemyInterface---End

	// Temp Bool flag
	UPROPERTY(BlueprintReadOnly)
	bool bHighlightActor = false;
};
