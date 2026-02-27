

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
//#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"

#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

/**
 * Enum to decide how the Gameplay Effect should be applied
 */
UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

/**
 * Enum to decide how the Gameplay Effect should be removed
 * Should only be applied on an Infinite Gameplay Effect
 */
UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

/**
 * Actor designed to apply an effect on another actor
 */
UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();
protected:
	virtual void BeginPlay() override;
	
	// Method to apply a Gameplay Effect to a target
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffect);
	
	UFUNCTION(BlueprintCallable)
	// Called when this actor is overlapped and decides what to do based on EEffectApplicationPolicy
	void OnOverlap(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable)
	// Called when this actor is no longer overlapped and decides what to do based on EEffectApplicationPolicy 
	// Note: If the effect is infinite might use the EEffectRemovalPolicy to decide what to do
	void OnEndOverlap(AActor* TargetActor);
	
	// Should the actor be destroyed when the effect is removed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	bool bDestroyOnEffectRemoval = false;
	
	//~ Begin Instant Gameplay Effect
	// Gameplay Effect class to apply Instantly
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	
	// Policy on how to apply the Instant Gameplay Effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	//~ End Instant Gameplay Effect
	
	//~ Begin Duration Gameplay Effect
	// Gameplay Effect class to apply with a Duration
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	
	// Policy on how to apply the Duration Gameplay Effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	//~ End Duration Gameplay Effect
	
	//~ Begin Infinite Gameplay Effect
	// Gameplay Effect class to apply with a Duration
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	
	// Policy on how to apply the Infinite Gameplay Effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	// Policy on how to remove the Infinite Gameplay Effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;
	//~ End Infinite Gameplay Effect
	
	// Map the active Gameplay Effect Handles with the appropriate ASC
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHanles;
	
	// This actor's level
	// Will have implications on the effect's magnitude
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	float ActorLevel = 1.f;
};
