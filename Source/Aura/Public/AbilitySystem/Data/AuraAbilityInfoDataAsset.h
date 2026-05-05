

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraAbilityInfoDataAsset.generated.h"

// Structure to store the Ability's information
USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()
	
	// This Ability's tag
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();
	
	// This Ability's input tag to identify which input triggers this ability
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();
	
	// Cooldown tag for this ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag();
	
	// This Ability's icon to use in the UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;
	
	// This Ability's Background material to be displayed underneath the icon in the UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;
};


/**
 * Data Asset to store Ability information to use on the UI
 */
UCLASS()
class AURA_API UAuraAbilityInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// Information on all available Abilities and how to display them in the UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilityInformation")
	TArray<FAuraAbilityInfo> AbilityInformation;
	
	// Finds and Ability's Info by searching for its Ability Tag
	FAuraAbilityInfo FindAuraAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
};
