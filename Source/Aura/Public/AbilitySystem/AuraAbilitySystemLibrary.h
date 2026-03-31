

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAuraCharacterClassInfoDataAsset;
enum class EAuraCharacterClass : uint8;
class UAuraAttributeMenuController;
class UAuraOverlayController;
class UAbilitySystemComponent;
/**
 * Library in charge of auxiliary methods for the Aura Ability System
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	// Get the Overlay Controller
	// Assumes it's called from an autonomous client
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemController|WidgetController")
	static UAuraOverlayController* GetOverlayController(const UObject* WorldContextObject);
	
	// Get the Overlay Controller
	// Assumes it's called from an autonomous client
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemController|WidgetController")
	static UAuraAttributeMenuController* GetAttributeMenuController(const UObject* WorldContextObject);
	
	// Initializes the Default Attributes relating to this RPG Class
	// It reads this information from the UAuraCharacterClassInfoDataAsset stored in the AuraGameMode 
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemController|Character Class Defaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, EAuraCharacterClass CharacterClass, float Level, UAbilitySystemComponent*
	                                        AbilitySystemComponent);
	
	// Give to the ability system component's owner its startup abilities 
	// Note: Should be called on Server
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemController|Character Class Defaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent);
	
	// Retrieves the Character Class Info Data Asset
	// Note: Should be called on Server
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemController|Character Class Defaults")
	static UAuraCharacterClassInfoDataAsset* GetCharacterClassInfoDataAsset(const UObject* WorldContextObject);
};
