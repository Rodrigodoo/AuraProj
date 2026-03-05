

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "AuraMMC_MaxMana.generated.h"

/**
 * Modifier Magnitude Calculation class for Max Mana
 */
UCLASS()
class AURA_API UAuraMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UAuraMMC_MaxMana();
	
	//~ Begin - Methods from UGameplayModMagnitudeCalculation
	
	// Method responsible for executing the calculations for this modifier
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	//~ End - Methods from UGameplayModMagnitudeCalculation
	
private:
	// Captured Intelligence attribute
	// FGameplayEffectAttributeCaptureDefinition - struct for captures attributes
	FGameplayEffectAttributeCaptureDefinition IntelligenceDefinition;
};
