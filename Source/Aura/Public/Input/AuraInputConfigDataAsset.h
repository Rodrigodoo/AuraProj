

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraInputConfigDataAsset.generated.h"

/*
 * Struct to pair Input Action to Gameplay tags
 */
USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()
	
	// Input Action used by this input
	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;
	
	// Gameplay tag associated with this input
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * Data Asset to configure this project's input
 */
UCLASS()
class AURA_API UAuraInputConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;
	
	// Input action bindings to gameplay tags
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraInputAction> AbilityInputActions;
};
