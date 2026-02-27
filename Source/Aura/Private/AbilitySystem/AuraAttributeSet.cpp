


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EditorCategoryUtils.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(10.f);
	InitMaxMana(50.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Registers the variables to be replicated
	
	// Without conditions
	// Replicating everytime it's set (Opposite to REPNOTIFY_OnChange, which only replicates if value changed)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
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
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	// Notifies the Gameplay Attribute System of the change on this attribute
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
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
