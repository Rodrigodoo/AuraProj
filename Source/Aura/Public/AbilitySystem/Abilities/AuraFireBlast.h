

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"

/**
 * Ability that shoots a fire bolts in several directions
 */
UCLASS()
class AURA_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	// Get the Fire Balls Count for the ability level
	int32 GetFireBallsCount(const int32 AbilityLevel) const;
	
	// Get the Fire Balls Count for the current ability level
	UFUNCTION(BlueprintPure)
	int32 GetCurrentFireBallsCount() const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	FScalableFloat FireBalls;
};
