

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AuraAIController.generated.h"

class UBehaviorTreeComponent;

/**
 * Controller for AI (Enemy) pawns.
 * It takes care of AI behaviour via Behaviour Tree Component and Blackboard Component 
 */
UCLASS()
class AURA_API AAuraAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AAuraAIController();
	
protected:
	// Behavior Tree Component to run behavior trees
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
