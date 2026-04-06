

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAbilitySystemComponent;
class UAuraAttributeMenuController;
class UAuraCharacterClassInfoDataAsset;
class UAuraOverlayController;
enum class EAuraCharacterClass : uint8;
struct FGameplayEffectContextHandle;

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
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController")
	static UAuraOverlayController* GetOverlayController(const UObject* WorldContextObject);
	
	// Get the Overlay Controller
	// Assumes it's called from an autonomous client
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController")
	static UAuraAttributeMenuController* GetAttributeMenuController(const UObject* WorldContextObject);
	
	// Initializes the Default Attributes relating to this RPG Class
	// It reads this information from the UAuraCharacterClassInfoDataAsset stored in the AuraGameMode 
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, EAuraCharacterClass CharacterClass, float Level, UAbilitySystemComponent*
	                                        AbilitySystemComponent);
	
	// Give to the ability system component's owner its startup abilities 
	// Note: Should be called on Server
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent);
	
	// Retrieves the Character Class Info Data Asset
	// Note: Should be called on Server
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UAuraCharacterClassInfoDataAsset* GetCharacterClassInfoDataAsset(const UObject* WorldContextObject);
	
	// Checks if the Gameplay Effect pertains to a Blocked Hit
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	// Checks if the Gameplay Effect pertains to a Critical Hit
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	// Sets the Gameplay Effect to record a Blocked Hit
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);
	
	// Sets the Gameplay Effect to record a Critical Hit
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
};
