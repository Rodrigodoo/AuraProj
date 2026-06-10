
#pragma once

#include "GameplayEffectTypes.h"
#include "ScalableFloat.h"

#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;
// Debuff struct to gather all the debuffs cause by a certain damage type
USTRUCT(BlueprintType)
struct FAuraDebuff
{
	GENERATED_BODY()
	
	// Percentage chance to cause the debuff
	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	float DebuffChance = 0.f;
	
	// Damage cause by the debuff
	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	float DebuffDamage = 0.f;
	
	// Frequency that the debuff triggers
	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	float DebuffFrequency = 0.f;
	
	// Duration of the Debuff
	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	float DebuffDuration = 0.f;
};

// Damage struct that holds the damage caused by this ability with a specific type and the debuffs incurred
USTRUCT(BlueprintType)
struct FAuraDamage
{
	GENERATED_BODY()
	
	// Damage for this type
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage = 1.f;
	
	// Debuff for this type
	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	FAuraDebuff Debuff = FAuraDebuff();
	
	// Return a scaled value for the respective level
	float GetValueAtLevel(const float Level, const FString* ContextString = nullptr) const;
};

// Struct to hold all damage effect parameters.
// This will be used to pass damage parameters through the Ability System
USTRUCT(BlueprintType)
struct FAuraDamageEffectParams
{
	GENERATED_BODY()
	
	FAuraDamageEffectParams(){};
	
	// Gameplay effect associated with this damage
	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;
	
	// Source Ability System Component for this damage
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;
	
	// Target Ability System Component for this damage
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;
	
	// Total base damage (sum of all damage types)
	UPROPERTY()
	float TotalBaseDamage = 0.f;
	
	// Level of the Ability causing this damage
	UPROPERTY()
	int32 AbilityLevel = 1;
	
	// Map of all damage types (base damage + debuffs)
	UPROPERTY()
	TMap<FGameplayTag, FAuraDamage> DamageTypes;
};

// Struct to hold specific gameplay effect contexts
USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_USTRUCT_BODY()
	
	// bIsBlockedHit Getter and Setter 
	bool IsBlockedHit() const	{ return bIsBlockedHit;	}
	void SetIsBlockedHit(const bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit;	}

	// bIsCriticalHit Getter and Setter 
	bool IsCriticalHit() const { return bIsCriticalHit; }
	void SetIsCriticalHit(const bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	
	//~ Begin - FGameplayEffectContext overrides
	
	// Returns the actual struct used for serialization, subclasses must override this!
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
	
	// Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const override
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	// Custom serialization, subclasses must override this
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	
	//~ End - FGameplayEffectContext overrides
	
protected:
	// Signals if Effect was a Blocked Hit
	UPROPERTY()
	bool bIsBlockedHit = false;

	// Signals if Effect was a Critical Hit
	UPROPERTY()
	bool bIsCriticalHit = false;
	
};

// Template to establish stuct operations available
template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true // Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};