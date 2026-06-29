

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraBeamSpell.generated.h"

/**
 * Ability that spawns a beam of electricity to the enemy. 
 * This beam will be electrocuting the enemy
 */
UCLASS()
class AURA_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
	
public:
	// Caches the information from the Hit Result
	// Hit Location, Target Actor, ...
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);
	
	// Caches the owner's player controller
	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();
protected:
	// Location of where the beam should hit
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	FVector MouseHitLocation;
	
	// Actor to be hit by the beam
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<AActor> MouseHitActor;
	
	// The owner's player controller
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;
	
	// The owner's character
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<ACharacter> OwnerCharacter;
};
