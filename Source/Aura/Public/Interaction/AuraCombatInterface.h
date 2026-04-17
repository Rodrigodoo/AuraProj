

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "AuraCombatInterface.generated.h"

class UNiagaraSystem;
// Structure to associate a Montage info to a GameplayTag
// This will be used to decide what montages are played, which sockets are used, sounds to be played,
// and which tags are going to trigger those montages.
USTRUCT(blueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()
	
	// Montage to play
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TaggedMontage")
	UAnimMontage* Montage = nullptr;
	
	// Montage tag
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TaggedMontage")
	FGameplayTag MontageTag = FGameplayTag();
	
	// Socket to be used by this montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TaggedMontage")
	FName SocketName = NAME_None;
	
	// Sound to be played on impact caused by this montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TaggedMontage")
	USoundBase* ImpactSound = nullptr;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UAuraCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface to be used by all elements involved in combat
 */
class AURA_API IAuraCombatInterface
{
	GENERATED_BODY()

public:
	// Get the current level of the actor
	virtual int32 GetCharacterLevel() const;
	
	// Retrieves the combat socket location, generally for weapons, but depends on the montage being played
	// This can be used to spawn effects, actor like projectiles, or apply damage.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag) const;
	
	// Updates the Warp Motion Facing Target location.
	// This can be used to set warping motions like translation or rotation
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& TargetLocation);
	
	// Retrieve the Hit React Montage for this object
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();
	
	// Method to be called when object/actor is destroyed/dies
	virtual void Die() = 0;
	
	// Checks if this interface owner is dead
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;
	
	// Get a reference to this interface owner
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();
	
	// Retrieves all attack montages used by this actor
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages() const;
	
	// Retrieves a random attack montage used by this actor
	// Returns true if a valid montage was found
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool FindRandomAttackMontage(FTaggedMontage& RandomMontage) const;
	
	// Retrieves the tagged montage that uses the provided montage tag
	// Returns true if a valid montage was found
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool FindTaggedMontageByTag(const FGameplayTag& MontageTag, FTaggedMontage& OutTaggedMontage) const;
	
	// Retrieves the Blood Effect for this actor
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();
};
