

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraAbilityInfoDataAsset.generated.h"

class UGameplayAbility;

// Structure to store the Ability's information
USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()
	
	// This Ability's tag
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Abilities"))
	FGameplayTag AbilityTag = FGameplayTag();
	
	// Cooldown tag for this ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Cooldown"))
	FGameplayTag CooldownTag = FGameplayTag();
	
	// Type tag for this ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Abilities.Type"))
	FGameplayTag TypeTag = FGameplayTag();
	
	// This Ability's description (To be used as Rich Text)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine="true"))
	FString Description = FString();
	
	// This Ability's next level description (To be used as Rich Text)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  meta = (MultiLine="true"))
	FString NextLevelDescription = FString();
	
	// This Ability's icon to use in the UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;
	
	// This Ability's Background material to be displayed underneath the icon in the UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;
	
	// The character level required to unlock this ability
	// Even tough the ability is unlocked the player must still "purchase" the ability to use it
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement = 1;
	
	// This Ability's class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityClass;
	
	// Read-Only Properties
	// This Ability's input tag to identify which input triggers this ability
	UPROPERTY(BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag = FGameplayTag();
	
	// Status tag for this ability
	UPROPERTY(BlueprintReadOnly, meta = (Categories = "Abilities.Status"))
	FGameplayTag StatusTag = FGameplayTag();
	
	// Retrieves the description with all the data filled in
	FString GetDescription(int32 Level) const;
	// Retrieves the next level description with all the data filled in
	FString GetNextLevelDescription(int32 Level) const;
	// Retrieves the locked description with all the data filled in
	FString GetLockedDescription() const;
	
private:
	// Retrieves the data to implement on the texts
	void FormatTextValues(FString& OutText, const int32 Level, bool bNextLevel = false) const;
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
