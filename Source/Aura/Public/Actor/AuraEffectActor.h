

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;
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
	void ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffect) const;
	
	// Gameplay Effect class to apply 
	UPROPERTY(EditAnywhere, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
};
