

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AuraEnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAuraEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface is to be used by enemies to apply generalized behaviour
 * NOTE: If other object are also to be highlighted then create a new HighlightInterface and make this one inherit from it!
 */
class AURA_API IAuraEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Highlights the actor
	virtual void HighlightActor() = 0;
	
	// Unhighlights the actor
	virtual void UnHighlightActor() = 0;
};
