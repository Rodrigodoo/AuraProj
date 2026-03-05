


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Registers the variables to be replicated
	
	// Without conditions
	// Replicating everytime it's set (Opposite to REPNOTIFY_OnChange, which only replicates if value changed)
	
	/*
	 * Primary Attributes
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	
	/*
	 * Secondary Attributes
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	
	/*
	 * Vital Attributes (These should only be initialized after MaxHealth & MaxMana)
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	// Clamp Health
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	// Clamp Mana
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	// Fill in the properties
	FEffectProperties EffectProperties;
	SetEffectProperties(Data,EffectProperties);
	
	// Clamp Health
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	
	// Clamp Mana
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,
                                            FEffectProperties& OutEffectProperties) const
{
	// This will be a learning exercise to know all important info stored inside of Data.
	// This Code is NOT optimized and exists only to show several options on how to retrieve the data
	
	// Source = causer of effect | Target = target of the effect (owner of this Attribute Set)
	
	//~ Context Handle
	OutEffectProperties.EffectContextHandle = Data.EffectSpec.GetContext();
	
	//~ Source Ability System Component
	OutEffectProperties.SourceAbilitySystemComponent = OutEffectProperties.EffectContextHandle.GetInstigatorAbilitySystemComponent();
	if (IsValid(OutEffectProperties.SourceAbilitySystemComponent))
	{
		// Check info inside AbilityActorInfo
		if (OutEffectProperties.SourceAbilitySystemComponent ->AbilityActorInfo.IsValid())
		{
			//~ Source Avatar Actor
			// Could also use SourceAsc->AbilityActorInfo->AvatarActor, but this is a better method. Otherwise, add checks.
			OutEffectProperties.SourceAvatarActor = OutEffectProperties.SourceAbilitySystemComponent ->GetAvatarActor();

			//~ Source Controller
			// in this case we do not have an accessor function so we need to do the proper checks
			if (OutEffectProperties.SourceAbilitySystemComponent ->AbilityActorInfo->PlayerController.IsValid())
			{
				OutEffectProperties.SourceController = OutEffectProperties.SourceAbilitySystemComponent->AbilityActorInfo->PlayerController.Get();
			}
			// If the AbilityActorInfo does not have a valid ptr but there is an avatar actor (APawn) we try to get the Player controller from it
			else if (const APawn* Pawn = Cast<APawn>(OutEffectProperties.SourceAvatarActor))
			{
				OutEffectProperties.SourceController = Pawn->GetController();
			}
			
			//~ Source Character
			if (OutEffectProperties.SourceController)
			{
				OutEffectProperties.SourceCharacter = OutEffectProperties.SourceController->GetCharacter();
			}
		}
		
		//~ Target Actor
		// By using GetAvatarActor you do not need any more checks
		OutEffectProperties.TargetAvatarActor = Data.Target.GetAvatarActor();
		
		//~ Target Controller
		// Here we need to do proper checks again
		if (Data.Target.AbilityActorInfo.IsValid())
		{
			if (Data.Target.AbilityActorInfo->PlayerController.IsValid())
			{
				OutEffectProperties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
			}
			// If the AbilityActorInfo does not have a valid ptr but there is an avatar actor (APawn) we try to get the Player controller from it
			else if (const APawn* Pawn = Cast<APawn>(OutEffectProperties.TargetAvatarActor))
			{
				OutEffectProperties.TargetController = Pawn->GetController();
			}
		}
		
		//~ Target Character
		// We could use one of the two methods to get the character
		if (OutEffectProperties.TargetController)
		{
			OutEffectProperties.TargetCharacter = OutEffectProperties.TargetController->GetCharacter();
		}
		else
		{
			OutEffectProperties.TargetCharacter = Cast<ACharacter>(OutEffectProperties.TargetAvatarActor);
		}
		
		//~ Target ASC
		// Cannot use |Data.Target| as it is a class and not a ptr to the actual object
		// Using UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent is the best method to get it
		OutEffectProperties.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OutEffectProperties.TargetAvatarActor);
	}
}
