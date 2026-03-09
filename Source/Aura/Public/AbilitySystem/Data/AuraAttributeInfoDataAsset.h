

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraAttributeInfoDataAsset.generated.h"

/*
 * Structure to store and pass attribute's data.
 * This struct can be sent through the system to display Attribute information
 */
USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()
	
	// Gameplay Tag to identify the Attribute
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();
	
	// The Attribute's name
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();
	
	// The Attribute's description
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();
	
	// The Attribute's value
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.0f;
};

/**
 * Aura Attribute Info Data Asset - used to store information about specific attributes
 */
UCLASS()
class AURA_API UAuraAttributeInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// Finds the Attribute Info for the Attribute identified by AttributeTag
	// If bLogNotFound = true, a log will be produced if the Attribute info is not found
	FAuraAttributeInfo FindAttributeInfoFromTag(const FGameplayTag& AttributeTag, const bool bLogNotFound = false) const;
	
	// Contains the Attribute information for all the project's attributes
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAttributeInfo> AttributeInformation;
};
