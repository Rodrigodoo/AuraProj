

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/AuraSpellMenuController.h"
#include "AuraAbilitySystemLibrary.generated.h"

class AAuraHUD;
struct FGameplayAbilitySpec;
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
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static UAuraOverlayController* GetOverlayController(const UObject* WorldContextObject);
	
	// Get the Attribute Menu Controller
	// Assumes it's called from an autonomous client
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static UAuraAttributeMenuController* GetAttributeMenuController(const UObject* WorldContextObject);
	
	// Get the Spell Controller
	// Assumes it's called from an autonomous client
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static UAuraSpellMenuController* GetSpellMenuController(const UObject* WorldContextObject);
	
	// Initializes the Default Attributes relating to this RPG Class
	// It reads this information from the UAuraCharacterClassInfoDataAsset stored in the AuraGameMode 
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta=(DefaultToSelf = "WorldContextObject"))
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, EAuraCharacterClass CharacterClass, 
		float Level, UAbilitySystemComponent*AbilitySystemComponent);
	
	// Give to the ability system component's owner its startup abilities 
	// Note: Should be called on Server
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta=(DefaultToSelf = "WorldContextObject"))
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, 
		EAuraCharacterClass CharacterClass);
	
	// Retrieves the Character Class Info Data Asset
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta=(DefaultToSelf = "WorldContextObject"))
	static UAuraCharacterClassInfoDataAsset* GetCharacterClassInfoDataAsset(const UObject* WorldContextObject);
	
	// Retrieves the Ability Info Data Asset
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta=(DefaultToSelf = "WorldContextObject"))
	static UAuraAbilityInfoDataAsset* GetAbilityInfoDataAsset(const UObject* WorldContextObject);
	
	// Retrieves the Ability Info from the Ability Info Data Asset
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta=(DefaultToSelf = "WorldContextObject"))
	static FAuraAbilityInfo FindAbilityInfoFromTag(const UObject* WorldContextObject, const FGameplayTag& AbilityTag);
	
	// Retrieves the Ability Description and Next Level Description
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta=(DefaultToSelf = "WorldContextObject"))
	static bool GetAbilityDescriptions(const UObject* WorldContextObject, const FGameplayTag& AbilityTag, FString& OutDescription,
	                                   FString& OutNextLevelDescription, const int32 Level = 1,
	                                   bool bUseLockedDescription = false);
	
	// Finds how much XP should be rewarded when killing a character of said class and level
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, EAuraCharacterClass CharacterClass, const int32 CharacterLevel);
	
	// Retrieves the Ability's tag from its Ability Spec
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	// Retrieves the Ability's Input tag from its Ability Spec
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	// Retrieves the Ability's Status from to its Ability Spec
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	// Retrieves the Ability's Type from to its Ability Spec
	static FGameplayTag GetTypeFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	// Clear the Input Tag from this Ability Spec if it exists
	static bool ClearInputTagFromSpec(FGameplayAbilitySpec& AbilitySpec);
	
	// checks if this Ability has a specific Input Tag
	static bool AbilityHasInputTag(const FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& InputTagToCheck);
	
	// Assign a Tag set by caller magnitude and apply it to self
	// Note: If SupressWarnings = true, then a set by caller magnitude will be created for all other modifiers 
	// that use set by caller with a magnitude of 0.f. This will guarantee that no warning will occur.
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Effects")
	static void AssignAndApplyToSelfSetByCallerEffect(UAbilitySystemComponent* AbilitySystemComponent,
	                                            const FGameplayEffectSpecHandle& EffectSpecHandle,
	                                            const FGameplayTag& DataTag, float Magnitude,
	                                            bool SupressWarnings = true);
	
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
	
	// Searches for live players within a certain radius from an origin point
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics", meta=(DefaultToSelf = "WorldContextObject"))
	static void GetLivePlayerWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, 
		const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& OriginPoint, bool DebugSphere = false);
	
	// Checks if the actors are not friendly towards each-other
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);
	
	// Retrieves a random spawn location in the provided line
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static FVector GetRandomLocationInLine(const FVector& Origin, const FVector& Direction, const float MinDistance, float MaxDistance);
	
	// Check if Child is actually a child of Parent.
	static void CheckIsClassChildOf(const UClass* Parent, const UClass* Child);
	
private:
	// Creates Controller Parameters to be used in creating Widget Controllers
	// Also returns a reference to the HUD, retrieved from the player controller
	// Note: private method
	static FWidgetControllerParams MakeWidgetControllerParams(const UObject* WorldContextObject, AAuraHUD*& AuraHUD);
	
};
