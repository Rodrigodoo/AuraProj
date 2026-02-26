


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffect)
{
	// Better getter function as it will cast for the IAbilitySystemInterface, 
	// and if it doesn't work will then look for all the actor's components to find the ASC
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor); 
	if (!TargetASC)
	{
		// Early exit
		return;
	}
	
	// If the gameplay effect class is not set we should crash
	check(GameplayEffect)
	
	// Create a Context Handle
	FGameplayEffectContextHandle GameplayEffectContextHandle = TargetASC->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(this);
	
	// Create and Effect Spec Handle
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffect, 1.0f, GameplayEffectContextHandle);
	
	// Apply the effect spec to self
	const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
	// This is for academic purposes to learn how to check a policy at runtime (we should not change the policy, only read)
	// EffectSpecHandle -> TSharedPtr<FGameplayEffectSpec> -> FGameplayEffectSpec* -> TObjectPtr<UGamePlayEffect> -> UGamePlayEffect* -> DurationPolicy
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		// If the effect is Infinite then store the active handle in a map for later (possible) termination
		ActiveEffectHanles.Add(ActiveGameplayEffectHandle,TargetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	// Check if the Instant Application Policy is set to ApplyOnOverlap
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		// If so apply the Instant Effect
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	
	// Check if the Duration Application Policy is set to ApplyOnOverlap
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		// If so apply the Duration Effect
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	
	// Check if the Infinite Application Policy is set to ApplyOnOverlap
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		// If so apply the Duration Effect
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	// Check if the Instant Application Policy is set to ApplyOnOverlap
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		// If so apply the Instant Effect
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	
	// Check if the Duration Application Policy is set to ApplyOnOverlap
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		// If so apply the Duration Effect
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	
	// Check if the Infinite Application Policy is set to ApplyOnOverlap
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		// If so apply the Duration Effect
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		// Remove the current infinite effect
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC))
		{
			// If it's not valid we are done
			return;
		}

		// Loop through the map to see if there are any ASC matching the target's ASC
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (auto HandlePair : ActiveEffectHanles)
		{
			// If there are, then remove its active Gameplay Effects
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		
		// Remove the Handle pairs from the map
		for (auto& Handle : HandlesToRemove)
		{
			ActiveEffectHanles.Remove(Handle);
		}
	}
}

