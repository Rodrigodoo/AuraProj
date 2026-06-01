

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbilityBase.generated.h"

/**
 * Aura's base Gameplay Ability class. Other Gameplay Abilities for this project should inherit from it.
 */
UCLASS()
class AURA_API UAuraGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	// Input Gameplay Tag for the startup ability
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (Categories = "InputTag"))
	FGameplayTag StartupInputTag;
	
	//~ Begin - Ability Description Getters
	
	// Getter for the description of this ability for the supplied level
	virtual FString GetDescription(const int32 Level) const;
	// Getter for the description of this ability at the next supplied level
	virtual FString GetNextLevelDescription(const int32 NextLevel) const;
	// Getter for the description of this ability at the next supplied level
	static FString GetLockedDescription(const int32 Level);
	
	//~ End - Ability Description Getters
};
