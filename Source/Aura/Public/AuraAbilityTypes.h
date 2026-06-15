
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
	// Was this Debuff successful (Should only be used by FAuraGameplayEffectContext)
	UPROPERTY()
	bool bIsSuccessfulDebuff = false;
	
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
	
	// Serialization method
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess);
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

// Struct wrapper to serialize the DamageTypeToDebuff map in FAuraGameplayEffectContext
USTRUCT(BlueprintType)
struct FDamageTypeToDebuffWrapper
{
	GENERATED_BODY()
	
	FDamageTypeToDebuffWrapper(){};
	explicit FDamageTypeToDebuffWrapper(const TMap<FGameplayTag, FAuraDebuff>& InDamageTypeToDebuff) 
	: DamageTypeToDebuff(InDamageTypeToDebuff) {};
	
	// Copy of damage type and associated debuff stats
	UPROPERTY()
	TMap<FGameplayTag, FAuraDebuff> DamageTypeToDebuff;
	
	// Serialization method
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess);
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
	
	// bIsSuccessfulDebuff Getter and Setter 
	bool IsSuccessfulDebuff(const FGameplayTag& DamageType) const;
	void SetSuccessfulDebuff(const FGameplayTag& DamageType, const bool bIsSuccessfulDebuff);
	bool WasAnyDebuffApplied() const;
	
	// DebuffDamage Getter and Setter
	float GetDebuffDamage(const FGameplayTag& DamageType) const;
	void SetDebuffDamage(const FGameplayTag& DamageType, const float InDamage);
	
	// DebuffFrequency Getter and Setter
	float GetDebuffFrequency(const FGameplayTag& DamageType) const;
	void SetDebuffFrequency(const FGameplayTag& DamageType, const float InFrequency);
	
	// DebuffDuration Getter and Setter
	float GetDebuffDuration(const FGameplayTag& DamageType) const;
	void SetDebuffDuration(const FGameplayTag& DamageType, const float InDuration);
	
	// DamageType Getter and Setter
	TArray<FGameplayTag> GetDamageTypes() const;
	// Note: this does not add a value!
	void AddDamageType(const FGameplayTag& InDamageType);
	void AddDamageTypeAndDebuff(const FGameplayTag& InDamageType, const FAuraDebuff& InDebuff);
	
	/*// bIsSuccessfulDebuff Getter and Setter 
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	void SetSuccessfulDebuff(const bool bIsSuccessfulDebuff) { bIsCriticalHit = bIsSuccessfulDebuff; }
	
	// DebuffDamage Getter and Setter
	float GetDebuffDamage() const { return DebuffDamage; }
	void SetDebuffDamage(const float InDamage) { DebuffDamage = InDamage; }
	
	// DebuffFrequency Getter and Setter
	float GetDebuffFrequency() const { return DebuffFrequency; }
	void SetDebuffFrequency(const float InFrequency) { DebuffFrequency = InFrequency; }
	
	// DebuffDuration Getter and Setter
	float GetDebuffDuration() const { return DebuffDuration; }
	void SetDebuffDuration(const float InDuration) { DebuffDuration = InDuration; }*/
	
	/*
	// DamageType Getter and Setter
	const FGameplayTag& GetDebuffType() const { return DamageType; }
	void SetDamageType(const FGameplayTag& InType) { DamageType = InType; }*/
	
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
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;
	
	//~ End - FGameplayEffectContext overrides
	
protected:
	// Signals if Effect was a Blocked Hit
	UPROPERTY()
	bool bIsBlockedHit = false;

	// Signals if Effect was a Critical Hit
	UPROPERTY()
	bool bIsCriticalHit = false;
	
	// Damage type and associated debuff stats
	UPROPERTY()
	TMap<FGameplayTag, FAuraDebuff> DamageTypeToDebuff;
	
	/*// Type of damage
	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();
	
	//~ Begin - Debuff
	FAuraDebuff DebuffStats = FAuraDebuff();*/
	/*// Signals if Effect was a Successful Debuff
	UPROPERTY()
	bool bIsSuccessfulDebuff = false;
	
	// Damage caused by debuff
	UPROPERTY()
	float DebuffDamage = 0.f;
	
	// Frequency of the debuff
	UPROPERTY()
	float DebuffFrequency = 0.f;
	
	// Duration of the debuff
	UPROPERTY()
	float DebuffDuration = 0.f;*/

	//~ End - Debuff
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

template<>
struct TStructOpsTypeTraits<FAuraDebuff> : TStructOpsTypeTraitsBase2<FAuraDebuff>
{
	enum
	{
		WithNetSerializer = true, // Tells the engine to look for NetSerialize()
		WithCopy = true
	};
};

template<>
struct TStructOpsTypeTraits<FDamageTypeToDebuffWrapper> : TStructOpsTypeTraitsBase2<FDamageTypeToDebuffWrapper>
{
	enum
	{
		WithNetSerializer = true, // Tells the engine to look for NetSerialize()
		WithCopy = true
	};
};