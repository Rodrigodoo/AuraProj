

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AuraCharacterClassInfoDataAsset.generated.h"

class UGameplayEffect;
/*
 * Enum to define the available Aura RPG Classes
 */
UENUM(BlueprintType)
enum class EAuraCharacterClass : uint8
{
	Warrior,
	Ranger,
	Elementalist
};

/*
 * Stores all the information for each Aura RPG Class
 */
USTRUCT(BlueprintType)
struct FAuraCharacterClassDefaultInfo
{
	GENERATED_BODY()
	
	// Gameplay effect to set the default Primary Attributes
	// Only Primary Attributes are class specific
	UPROPERTY(EditAnywhere, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
};

/**
 * Data Asset that stores:
 *  - Default Attributes per RPG Class
 *  - Abilities per RPG Class
 *  - Gameplay Effects per RPG Class
 *  RPG Classes : Warrior/Ranger/Elementalist
 */
UCLASS()
class AURA_API UAuraCharacterClassInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	// Map to associate each Aura RPG Class to its information struct
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<EAuraCharacterClass, FAuraCharacterClassDefaultInfo> CharacterClassInformation;
	
	// Gameplay effect to set the default Secondary Attributes
	// Secondary Attribute calculation is shared between all classes
	UPROPERTY(EditAnywhere, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;
	
	// Gameplay effect to set the default Vital Attributes
	// Vital Attribute calculation is shared between all classes
	UPROPERTY(EditAnywhere, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;
	
	// Looks through the CharacterClassInformation map for the class Default Info for this class
	// Note: It will assert if it doesn't find an entry
	FAuraCharacterClassDefaultInfo GetClassDefaultInfo(EAuraCharacterClass CharacterClass) const;
};
