

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "AuraCombatInterface.generated.h"

class UAbilitySystemComponent;
enum class ECharacterClass : uint8;
class UNiagaraSystem;

// Delegates
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilitySystemComponentRegistered, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeathActor);

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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetCharacterLevel() const;
	
	// Retrieves the Character's class.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EAuraCharacterClass GetCharacterClass() const;
	
	// Retrieves the combat socket location, generally for weapons, but depends on the montage being played
	// This can be used to spawn effects, actor like projectiles, or apply damage.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag) const;
	
	// Updates the Warp Motion Facing Target location.
	// This can be used to set warping motions like translation or rotation
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& TargetLocation);

	// Method to be called when object/actor is destroyed/dies
	virtual void Die() = 0;
	
	// Checks if this interface owner is dead
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;
	
	// Get a reference to this interface owner
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();
	
	// Retrieves the Blood Effect for this actor
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();
	
	// Retrieve the Ability System Component delegate for when it is implemented
	virtual FOnAbilitySystemComponentRegistered& GetAbilitySystemComponentRegisteredDelegate() = 0;
	
	// Retrieve the On Death delegate (Delegate called whenever the character dies)
	virtual FOnDeathSignature& GetOnDeathDelegate() = 0;
	
	//~ Begin - Montages
	
	// Retrieve the Hit React Montage for this object
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();
	
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
	
	//~ End - Montages
	
	
	//~ Begin - Minions
	
	// Retrieves the number of alive minions bound to this object
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount() const;
	
	// Set the minion count for the alive minions bound to this object
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMinionCount(const int32 NewMinionCount);
	
	// Increment the minion count by a certain amount for the alive minions bound to this object
	// Negative numbers will not be considered
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (UIMin = "1"))
	void IncrementMinionCount(int32 Amount = 1);
	
	// Decrement the minion count by a certain amount for the alive minions bound to this object
	// Negative numbers will not be considered
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (UIMin = "1"))
	void DecrementMinionCount(int32 Amount = 1);
	
	//~ End - Minions
};
