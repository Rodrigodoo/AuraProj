

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "AuraMMC_MaxHealth.generated.h"

/**
 * Modifier Magnitude Calculation class for Max Health
 */
UCLASS()
class AURA_API UAuraMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UAuraMMC_MaxHealth();
	
	//~ Begin - Methods from UGameplayModMagnitudeCalculation
	
	// Method responsible for executing the calculations for this modifier
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	//~ End - Methods from UGameplayModMagnitudeCalculation
	
private:
	
	// Captured Vigor attribute
	// FGameplayEffectAttributeCaptureDefinition - struct for captures attributes
	FGameplayEffectAttributeCaptureDefinition VigorDefinition;
};
