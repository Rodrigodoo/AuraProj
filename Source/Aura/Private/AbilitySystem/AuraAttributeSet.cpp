


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTagsManager.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/AuraCombatInterface.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	// Map Gameplay Tags to the Attributes
	MapGameplayTagsToAttributes();
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
	 * Resistance Attributes
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	
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
		UE_LOG(LogTemp, Warning, TEXT("Changed Health on %s: Health -> %f"), *EffectProperties.TargetAvatarActor->GetName(), GetHealth());
	}
	
	// Clamp Mana
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	
	// Process any Incoming Damage
	// Note: This only gets called on the server
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		// Get the incoming damage
		const float LocalIncomingDamage = GetIncomingDamage();
		
		// Reset the Incoming Damage to process new sources of damage
		SetIncomingDamage(0.f);
		
		// There was actual damage being applied
		if (LocalIncomingDamage > 0)
		{
			// For now apply directly the damage to health
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			
			// Damage was fatal
			if (NewHealth <= 0.f)
			{
				// If the object dying has a combat interface call its Die method
				if (IAuraCombatInterface* CombatInterface = Cast<IAuraCombatInterface>(EffectProperties.TargetAvatarActor))
				{
					CombatInterface->Die();
				}
			}
			else // No fatal damage
			{
				// Activate any Ability that has the Hit React Tag
				const FGameplayTagContainer AbilityTagContainer(AuraGameplayTagsManager::Effects_HitReact);
				EffectProperties.TargetAbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTagContainer);
			}

			// Display the Damage applied to the Target
			const bool bBlockedHit = UAuraAbilitySystemLibrary::IsBlockedHit(EffectProperties.EffectContextHandle);
			const bool bCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(EffectProperties.EffectContextHandle);
			ShowDamageAsFloatingText(EffectProperties, LocalIncomingDamage, bBlockedHit, bCriticalHit);
		}
	}
}

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldFireResistance);
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldLightningResistance);
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldArcaneResistance);
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldPhysicalResistance);
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
		if (OutEffectProperties.SourceAbilitySystemComponent->AbilityActorInfo.IsValid())
		{
			//~ Source Avatar Actor
			// Could also use SourceAsc->AbilityActorInfo->AvatarActor, but this is a better method. Otherwise, add checks.
			OutEffectProperties.SourceAvatarActor = OutEffectProperties.SourceAbilitySystemComponent ->GetAvatarActor();

			//~ Source Controller
			// in this case we do not have an accessor function so we need to do the proper checks
			if (OutEffectProperties.SourceAbilitySystemComponent->AbilityActorInfo->PlayerController.IsValid())
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

void UAuraAttributeSet::MapGameplayTagsToAttributes()
{
	/*
	 * Primary Attributes
	 */
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Primary_Strength, GetStrengthAttribute);
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Primary_Resilience, GetResilienceAttribute);
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Primary_Vigor, GetVigorAttribute);

	/*
	 * Secondary Attributes
	 */
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Secondary_Armor, GetArmorAttribute);
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Secondary_MaxMana, GetMaxManaAttribute);
	
	/*
	 * Resistance Attributes
	 */
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Resistance_Fire, GetFireResistanceAttribute);
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Resistance_Lightning, GetLightningResistanceAttribute);
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Resistance_Arcane, GetArcaneResistanceAttribute);
	TagToAttributes.Add(AuraGameplayTagsManager::Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);
}

void UAuraAttributeSet::ShowDamageAsFloatingText(const FEffectProperties& EffectProperties, const float Damage, const bool bBlockedHit, const bool bCriticalHit)
{
	// Early checks
	if (EffectProperties.SourceCharacter == EffectProperties.TargetCharacter)
	{
		return;
	}
	
	// If the damage is not being applied to Self
	// Then display the damage applied to the Target
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(EffectProperties.SourceController))
	{
		AuraPlayerController->ShowDamageNumber(Damage, EffectProperties.TargetCharacter, bBlockedHit, bCriticalHit);
	}
}
