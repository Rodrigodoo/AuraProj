


#include "AI/AuraAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AAuraAIController::AAuraAIController()
{
	// Create Default Objects
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComponent && Blackboard);
}
