
#pragma once

#include "GameplayEffectTypes.h"

#include "AuraAbilityTypes.generated.h"


USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_USTRUCT_BODY()
	
public:
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
		return FAuraGameplayEffectContext::StaticStruct();
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