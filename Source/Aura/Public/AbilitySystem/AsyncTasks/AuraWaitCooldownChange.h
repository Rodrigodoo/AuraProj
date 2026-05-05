

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AuraWaitCooldownChange.generated.h"

class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

/**
 * Task that waits for any change to an ability's cooldown (start, end, re-start, etc..)
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class AURA_API UAuraWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	// Delegate to be fired when cooldown starts
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;
	
	// Delegate to be fired when cooldown ends
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;
	
	// Factory method to creat a Wait for Cooldown Change Node
	// Receives and Ability System Component and a cooldown tag to listen to
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UAuraWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* InAbilitySystemComponent, FGameplayTag InCooldownTag);
	
	// Ends tasks and clears any lingering references
	UFUNCTION(BlueprintCallable)
	void EndTask();
protected:
	// Cached Ability System Component to listen for cooldown changes
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	// Cached Gameplay tag to identify the cooldown being listened to
	UPROPERTY()
	FGameplayTag CooldownTag;
	
	// Delegate to be called when a Cooldown Gameplay Tag is added or removed
	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
};
