
#include "AuraAbilityTypes.h"

#include "AuraGameplayTagsManager.h"


bool FAuraDebuff::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	// Bitmask
	uint32 RepBits = 0;
	
	// If Saving, signal in the bitmask what properties are to be saved
	if (Ar.IsSaving())
	{
		if (bIsSuccessfulDebuff)
		{
			RepBits |= 1 << 0;
		}
		if (DebuffChance > 0.f)
		{
			RepBits |= 1 << 1;
		}
		if (DebuffDamage > 0.f)
		{
			RepBits |= 1 << 2;
		}
		if (DebuffFrequency > 0.f)
		{
			RepBits |= 1 << 3;
		}
		if (DebuffDuration > 0.f)
		{
			RepBits |= 1 << 4;
		}
	}
	
	// Serialize/Deserialize the bit mask
	Ar.Serialize(&RepBits, 4);
	
	if (RepBits & (1 << 0))
	{
		Ar << bIsSuccessfulDebuff;
	}
	if (RepBits & (1 << 1))
	{
		Ar << DebuffChance;
	}
	if (RepBits & (1 << 2))
	{
		Ar << DebuffDamage;
	}
	if (RepBits & (1 << 3))
	{
		Ar << DebuffFrequency;
	}
	if (RepBits & (1 << 4))
	{
		Ar << DebuffDuration;
	}

	// Always set bOutSuccess to true if serialization completed without errors
	bOutSuccess = true; 
	return true;
}

float FAuraDamage::GetValueAtLevel(const float Level, const FString* ContextString) const
{
	return Damage.GetValueAtLevel(Level, ContextString);
}

bool FDamageTypeToDebuffWrapper::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	// Determine how many items are in the map
	int32 MapSize = DamageTypeToDebuff.Num();
	Ar << MapSize;

	if (Ar.IsLoading())
	{
		// Clear the local map before receiving incoming network data
		DamageTypeToDebuff.Empty(MapSize);

		// loop through the map and retrieve the networked data
		for (int32 i = 0; i < MapSize; ++i)
		{
			FGameplayTag Key;
			FAuraDebuff Value;

			// Deserialize key-value pairs out of the network archive buffer
			Ar << Key;
			Value.NetSerialize(Ar, Map, bOutSuccess);

			DamageTypeToDebuff.Add(Key, Value);
		}
	}
	else if (Ar.IsSaving())
	{
		// Serialize key-value pairs into the network archive buffer
		for (auto& Pair : DamageTypeToDebuff)
		{
			Ar << Pair.Key;
			Pair.Value.NetSerialize(Ar, Map, bOutSuccess);

		}
	}
	
	// Always set bOutSuccess to true if serialization completed without errors
	bOutSuccess = true; 
	return true;
}

FAuraDebuff FAuraGameplayEffectContext::GetDebuff(const FGameplayTag& DamageType) const
{
	if (!DamageTypeToDebuff.Contains(DamageType))
	{
		return FAuraDebuff();
	}
	
	return DamageTypeToDebuff[DamageType];
}

bool FAuraGameplayEffectContext::IsSuccessfulDebuff(const FGameplayTag& DamageType) const
{
	if (!DamageTypeToDebuff.Contains(DamageType))
	{
		return false;
	}
	
	return DamageTypeToDebuff[DamageType].bIsSuccessfulDebuff;
}

void FAuraGameplayEffectContext::SetSuccessfulDebuff(const FGameplayTag& DamageType, const bool bIsSuccessfulDebuff)
{
	if (DamageTypeToDebuff.Contains(DamageType))
	{
		DamageTypeToDebuff[DamageType].bIsSuccessfulDebuff = bIsSuccessfulDebuff;
	}
}

bool FAuraGameplayEffectContext::WasAnyDebuffApplied() const
{
	return DamageTypeToDebuff.Num() > 0;
}

float FAuraGameplayEffectContext::GetDebuffDamage(const FGameplayTag& DamageType) const
{
	if (!DamageTypeToDebuff.Contains(DamageType))
	{
		return -1.f;
	}
	
	return DamageTypeToDebuff[DamageType].DebuffDamage;
}

void FAuraGameplayEffectContext::SetDebuffDamage(const FGameplayTag& DamageType, const float InDamage)
{
	if (DamageTypeToDebuff.Contains(DamageType))
	{
		DamageTypeToDebuff[DamageType].DebuffDamage = InDamage;
	}
}

float FAuraGameplayEffectContext::GetDebuffFrequency(const FGameplayTag& DamageType) const
{
	if (!DamageTypeToDebuff.Contains(DamageType))
	{
		return -1.f;
	}
	
	return DamageTypeToDebuff[DamageType].DebuffFrequency;
}

void FAuraGameplayEffectContext::SetDebuffFrequency(const FGameplayTag& DamageType, const float InFrequency)
{
	if (DamageTypeToDebuff.Contains(DamageType))
	{
		DamageTypeToDebuff[DamageType].DebuffFrequency = InFrequency;
	}
}

float FAuraGameplayEffectContext::GetDebuffDuration(const FGameplayTag& DamageType) const
{
	if (!DamageTypeToDebuff.Contains(DamageType))
	{
		return -1.f;
	}
	
	return DamageTypeToDebuff[DamageType].DebuffDuration;
}

void FAuraGameplayEffectContext::SetDebuffDuration(const FGameplayTag& DamageType, const float InDuration)
{
	if (DamageTypeToDebuff.Contains(DamageType))
	{
		DamageTypeToDebuff[DamageType].DebuffDuration = InDuration;
	}
}

TArray<FGameplayTag> FAuraGameplayEffectContext::GetDamageTypes() const
{
	TArray<FGameplayTag> DamageTypes;
	DamageTypeToDebuff.GetKeys(DamageTypes);
	return DamageTypes;
}

void FAuraGameplayEffectContext::AddDamageType(const FGameplayTag& InDamageType)
{
	// Note: This does not add a value!
	DamageTypeToDebuff.Add(InDamageType);
}

void FAuraGameplayEffectContext::AddDamageTypeAndDebuff(const FGameplayTag& InDamageType, const FAuraDebuff& InDebuff)
{
	DamageTypeToDebuff.Add(InDamageType, InDebuff);
}

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	// Bitmask
	uint32 RepBits = 0;
	
	// If Saving, signal in the bitmask what properties are to be saved
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid() )
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (bIsBlockedHit)
		{
			RepBits |= 1 << 7;
		}
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 8;
		}
		if (WasAnyDebuffApplied())
		{
			RepBits |= 1 << 9;
		}
	}
	
	// Serialize/Deserialize the bit mask
	Ar.Serialize(&RepBits, 9);
	
	// Read the bitmask and Serialize/Deserialize the properties
	// Note: At this point we can either be loading or saving, depends on context
	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	
	if (RepBits & (1 << 7))
	{
		Ar << bIsBlockedHit;
	}
	if (RepBits & (1 << 8))
	{
		Ar << bIsCriticalHit;
	}
	if (RepBits & (1 << 9))
	{
		// Create a wrapper and serialize it
		FDamageTypeToDebuffWrapper DebuffWrapper(DamageTypeToDebuff);
		DebuffWrapper.NetSerialize(Ar, Map, bOutSuccess);
		
		// If we are loading we need to retrieve the map
		if (Ar.IsLoading())
		{
			DamageTypeToDebuff = DebuffWrapper.DamageTypeToDebuff;
		}
	}

	// If Loading, just initialize InstigatorAbilitySystemComponent
	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get());
	}
	
	bOutSuccess = true;
	return true;
}
