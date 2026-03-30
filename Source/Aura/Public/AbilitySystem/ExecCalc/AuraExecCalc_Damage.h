

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AuraExecCalc_Damage.generated.h"

/**
 * Custom calculation class to perform damage calculations
 */
UCLASS()
class AURA_API UAuraExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UAuraExecCalc_Damage();
	
	//~ Begin - UGameplayEffectExecutionCalculation overrides
	
	// Called when the Gameplay Effect is executed and will perform all calculations required
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	//~ End - UGameplayEffectExecutionCalculation overrides
	
};
