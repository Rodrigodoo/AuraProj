
#include "AuraAbilityTypes.h"

#include "AuraGameplayTagsManager.h"


float FAuraDamage::GetValueAtLevel(const float Level, const FString* ContextString) const
{
	return Damage.GetValueAtLevel(Level, ContextString);
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
		if (bIsSuccessfulDebuff)
		{
			RepBits |= 1 << 9;
		}
		if (DebuffDamage > 0.f)
		{
			RepBits |= 1 << 10;
		}
		if (DebuffFrequency > 0.f)
		{
			RepBits |= 1 << 11;
		}
		if (DebuffDuration > 0.f)
		{
			RepBits |= 1 << 12;
		}
		if (DamageType.IsValid() && DamageType.MatchesTag(AuraGameplayTagsManager::Debuff))
		{
			RepBits |= 1 << 13;
		}
	}
	
	// Serialize/Deserialize the bit mask
	Ar.Serialize(&RepBits, 13);
	
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
		Ar << bIsSuccessfulDebuff;
	}
	if (RepBits & (1 << 10))
	{
		Ar << DebuffDamage;
	}
	if (RepBits & (1 << 11))
	{
		Ar << DebuffFrequency;
	}
	if (RepBits & (1 << 12))
	{
		Ar << DebuffDuration;
	}
	if (RepBits & (1 << 13))
	{
		DamageType.NetSerialize(Ar, Map, bOutSuccess);
	}

	// If Loading, just initialize InstigatorAbilitySystemComponent
	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get());
	}
	
	bOutSuccess = true;
	return true;
}
