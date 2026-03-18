

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AuraCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UAuraCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface to be used by all elements involved in combat
 */
class AURA_API IAuraCombatInterface
{
	GENERATED_BODY()

public:
	// Get the current level of the actor
	virtual int32 GetCharacterLevel();
	
	// Retrieves the combat socket location, generally for weapons
	// This can be used to spawn effects or actor like projectiles
	virtual FVector GetCombatSocketLocation() const;
	
	// Updates the Warp Motion Facing Target location.
	// This can be used to set warping motions like translation or rotation
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& TargetLocation);
};
