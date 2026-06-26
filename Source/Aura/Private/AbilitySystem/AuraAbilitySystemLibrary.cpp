


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTagsManager.h"
#include "AbilitySystem/Data/AuraAbilityInfoDataAsset.h"
#include "AbilitySystem/Data/AuraCharacterClassInfoDataAsset.h"
#include "Engine/OverlapResult.h"
#include "Game/AuraGameInstance.h"
#include "Interaction/AuraCombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UAuraOverlayController* UAuraAbilitySystemLibrary::GetOverlayController(const UObject* WorldContextObject)
{
	// Make the Widget Controller params
	AAuraHUD* AuraHUD = nullptr;
	const FWidgetControllerParams ControllerParams = MakeWidgetControllerParams(WorldContextObject, AuraHUD);
	if (!ControllerParams.IsValid() || !AuraHUD)
	{
		// Early Exit
		return	nullptr;
	}
	
	// Retrieve the Overlay Controller (If one does not exist it will create one)
	return AuraHUD->GetAuraOverlayController(ControllerParams);
	
}

UAuraAttributeMenuController* UAuraAbilitySystemLibrary::GetAttributeMenuController(const UObject* WorldContextObject)
{
	// Make the Widget Controller params
	AAuraHUD* AuraHUD = nullptr;
	const FWidgetControllerParams ControllerParams = MakeWidgetControllerParams(WorldContextObject, AuraHUD);
	if (!ControllerParams.IsValid() || !AuraHUD)
	{
		// Early Exit
		return	nullptr;
	}
	
	return AuraHUD->GetAuraAttributeMenuController(ControllerParams);
}

UAuraSpellMenuController* UAuraAbilitySystemLibrary::GetSpellMenuController(const UObject* WorldContextObject)
{
	// Make the Widget Controller params
	AAuraHUD* AuraHUD = nullptr;
	const FWidgetControllerParams ControllerParams = MakeWidgetControllerParams(WorldContextObject, AuraHUD);
	if (!ControllerParams.IsValid() || !AuraHUD)
	{
		// Early Exit
		return	nullptr;
	}

	return AuraHUD->GetAuraSpellMenuController(ControllerParams);
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, const EAuraCharacterClass CharacterClass, float Level, UAbilitySystemComponent*
                                                            AbilitySystemComponent)
{
	// Get Character Class Info Data Asset
	const UAuraCharacterClassInfoDataAsset* CharacterClassInfoDataAsset = GetCharacterClassInfoDataAsset(WorldContextObject);
	if (!CharacterClassInfoDataAsset)
	{
		// If this is called on the client it will fail since GameMode is only available on the server
		return;
	}
	
	// Retrieve the Character Class Info for this specific RPG Class
	const FAuraCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfoDataAsset->GetClassDefaultInfo(CharacterClass);
	
	// Apply the Gameplay Effects to the provided Ability System Component
	
	// Get the Avatar actor to make as source Object
	const AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor(); 
	
	//~ Primary Attributes
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		ClassDefaultInfo.PrimaryAttributes, 
		Level, 
		PrimaryAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());
	
	//~ Secondary Attributes
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		CharacterClassInfoDataAsset->SecondaryAttributes, 
		Level, 
		SecondaryAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());
	
	//~ Vital Attributes
	FGameplayEffectContextHandle VitalAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		CharacterClassInfoDataAsset->VitalAttributes, 
		Level, 
		VitalAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
                                                     UAbilitySystemComponent* AbilitySystemComponent, EAuraCharacterClass CharacterClass)
{
	// Get Character Class Info Data Asset
	const UAuraCharacterClassInfoDataAsset* CharacterClassInfoDataAsset = GetCharacterClassInfoDataAsset(WorldContextObject);
	if (!CharacterClassInfoDataAsset)
	{
		// If this is called on the client it will fail since GameMode is only available on the server
		return;
	}
	
	// Get the character level to initialize the Abilities at the right level
	int32 CharacterLevel = 1;
	if (AbilitySystemComponent->GetAvatarActor()->Implements<UAuraCombatInterface>())
	{
		CharacterLevel = IAuraCombatInterface::Execute_GetCharacterLevel(AbilitySystemComponent->GetAvatarActor());
	}
	
	// Loop  through the common abilities and apply them to the Ability System Component's owner
	for (const TSubclassOf<UGameplayAbility> Ability : CharacterClassInfoDataAsset->CommonAbilities)
	{
		// Create an Ability Spec for this ability
		FGameplayAbilitySpec AbilitySpec(Ability, CharacterLevel);
		
		// Grant the ability
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
	
	// Go through the Character Class' specific startup abilities and apply them to the Ability System Component's owner
	FAuraCharacterClassDefaultInfo DefaultCharacterClassInfo = CharacterClassInfoDataAsset->GetClassDefaultInfo(CharacterClass);
	for (const TSubclassOf<UGameplayAbility> Ability : DefaultCharacterClassInfo.StartupAbilities)
	{
		// Create an Ability Spec for this ability
		FGameplayAbilitySpec AbilitySpec(Ability, CharacterLevel);
		
		// Grant the ability
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}

}

UAuraCharacterClassInfoDataAsset* UAuraAbilitySystemLibrary::GetCharacterClassInfoDataAsset(const UObject* WorldContextObject)
{
	// Get the game mode and the Character Class Info Data Asset
	const UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!AuraGameInstance)
	{
		// Something failed and the game instance is not available
		return nullptr;
	}
	
	check(AuraGameInstance->CharacterClassInfoDataAsset);
	return AuraGameInstance->CharacterClassInfoDataAsset;
}

UAuraAbilityInfoDataAsset* UAuraAbilitySystemLibrary::GetAbilityInfoDataAsset(const UObject* WorldContextObject)
{
	// Get the game mode and the Character Class Info Data Asset
	const UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!AuraGameInstance)
	{
		// Something failed and the game instance is not available
		return nullptr;
	}
	
	check(AuraGameInstance->AbilityInfoDataAsset);
	return AuraGameInstance->AbilityInfoDataAsset;
}

FAuraAbilityInfo UAuraAbilitySystemLibrary::FindAbilityInfoFromTag(const UObject* WorldContextObject,
	const FGameplayTag& AbilityTag)
{
	// Get the Ability info data asset and search for the ability
	const UAuraAbilityInfoDataAsset* AbilityInfoDataAsset = GetAbilityInfoDataAsset(WorldContextObject);
	if (!AbilityInfoDataAsset)
	{
		return FAuraAbilityInfo();
	}
	
	return AbilityInfoDataAsset->FindAuraAbilityInfoForTag(AbilityTag);
}

bool UAuraAbilitySystemLibrary::GetAbilityDescriptions(const UObject* WorldContextObject,
                                                       const FGameplayTag& AbilityTag, FString& OutDescription,
                                                       FString& OutNextLevelDescription, const int32 Level,
                                                       const bool bUseLockedDescription)
{
	// Early checks
	if (!AbilityTag.IsValid())
	{
		OutDescription = "Error - No Ability Tag Set!";
		OutNextLevelDescription = "";
		return false;
	}
	const FAuraAbilityInfo AbilityInfo = FindAbilityInfoFromTag(WorldContextObject, AbilityTag);
	if (bUseLockedDescription)
	{
		// If using the locked description pass it out via the description and leave the next level blank
		OutDescription = AbilityInfo.GetLockedDescription();
		OutNextLevelDescription = "";
		return false;
	}

	// Retrieve the description and the next level description
	OutDescription = AbilityInfo.GetDescription(Level);
	OutNextLevelDescription = AbilityInfo.GetNextLevelDescription(Level);
	return true;
}

int32 UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, const EAuraCharacterClass CharacterClass,
                                                             const int32 CharacterLevel)
{
	// Get Character Class Info Data Asset
	const UAuraCharacterClassInfoDataAsset* CharacterClassInfoDataAsset = GetCharacterClassInfoDataAsset(WorldContextObject);
	if (!CharacterClassInfoDataAsset)
	{
		// If this is called on the client it will fail since GameMode is only available on the server
		return 0;
	}

	// Find the appropriate CharacterClassDefaultInfo for this class type and return its XP Reward for the current level
	const FAuraCharacterClassDefaultInfo CharacterClassDefaultInfo = CharacterClassInfoDataAsset->GetClassDefaultInfo(CharacterClass);
	return static_cast<int32>(CharacterClassDefaultInfo.XPReward.GetValueAtLevel(CharacterLevel));
}

FGameplayTag UAuraAbilitySystemLibrary::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	// Early check
	if (!AbilitySpec.Ability->IsValidLowLevel())
	{
		return FGameplayTag();
	}
	
	// Loop through the Ability's tags and check if it has an Ability tag 
	// i.e: Checking for it in its parents tag. Example: Abilities.Firebolt or Abilities.Summon
	for (const FGameplayTag& Tag : AbilitySpec.Ability->GetAssetTags())
	{
		if (Tag.MatchesTag(AuraGameplayTagsManager::Abilities))
		{
			return Tag;
		}
	}
	
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemLibrary::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	// Loop through the Ability's dynamic tags and check if it has an Input tag 
	// i.e: Checking for it in its parents tag. Example: InputTag.LMB or InputTag.2
	for (const FGameplayTag& Tag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTag(AuraGameplayTagsManager::InputTag))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemLibrary::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	// Loop through the Ability's dynamic tags and check if it has a Status tag 
	// i.e: Checking for it in its parents tag. Example: Abilities.Status.Locked or Abilities.Status.Equipped
	for (const FGameplayTag& Tag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTag(AuraGameplayTagsManager::Abilities_Status))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemLibrary::GetTypeFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	// Loop through the Ability's dynamic tags and check if it has a Type tag 
	// i.e: Checking for it in its parents tag. Example: Abilities.Type.Offensive or Abilities.Type.None
	for (const FGameplayTag& Tag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTag(AuraGameplayTagsManager::Abilities_Type))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

bool UAuraAbilitySystemLibrary::ClearInputTagFromSpec(FGameplayAbilitySpec& AbilitySpec)
{
	// Get the Input Tag
	const FGameplayTag& InputTag = GetInputTagFromSpec(AbilitySpec);
	return AbilitySpec.GetDynamicSpecSourceTags().RemoveTag(InputTag);
}

bool UAuraAbilitySystemLibrary::AbilityHasInputTag(const FGameplayAbilitySpec& AbilitySpec,
                                                   const FGameplayTag& InputTagToCheck)
{
	// Get the Input Tag
	const FGameplayTag& InputTag = GetInputTagFromSpec(AbilitySpec);
	return InputTag.MatchesTagExact(InputTagToCheck);
}

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageGameplayEffect(const FAuraDamageEffectParams& Params)
{
	using namespace AuraGameplayTagsManager;
	
	// If there is no target or source, crash
	check(Params.TargetAbilitySystemComponent && Params.SourceAbilitySystemComponent)

	// Create a context handle and set its source object and Death impulse
	FGameplayEffectContextHandle ContextHandle = Params.SourceAbilitySystemComponent->MakeEffectContext();
	const AActor* SourceAvatarActor = Params.SourceAbilitySystemComponent->GetAvatarActor();
	ContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(ContextHandle, Params.DeathImpulse);
	SetKnockback(ContextHandle, Params.Knockback);

	// Create a spec handle to store all damage type values and then apply them to the Target Actor
	const FGameplayEffectSpecHandle DamageSpecHandle = Params.SourceAbilitySystemComponent->MakeOutgoingSpec(
		Params.DamageGameplayEffectClass, Params.AbilityLevel, ContextHandle);
	
	//  Key: GameplayTag (Damage Type tag) | FAuraDamage (Damage value and debuffs associated with type at a specific level)
	for (const auto& Pair: Params.DamageTypes)
	{
		// Local variable
		const FAuraDamage& AuraDamage = Pair.Value;
		
		// Create a tag set by caller magnitude for:

		// Damage type base value
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, AuraDamage.GetValueAtLevel(Params.AbilityLevel));
		
		// Retrieve the Debuff Type
		const FGameplayTag& DebuffType = DamageTypesToDebuffs[Pair.Key];
		
		// Debuffs *For now these debuffs only apply once (if several damage types)*
		// Debuff Chance
		FGameplayTag DebuffTag = GetDebuffChanceByType(DebuffType);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DebuffTag, AuraDamage.Debuff.DebuffChance);
		// Debuff Damage
		DebuffTag = GetDebuffDamageByType(DebuffType);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DebuffTag, AuraDamage.Debuff.DebuffDamage);
		// Debuff Frequency
		DebuffTag = GetDebuffFrequencyByType(DebuffType);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DebuffTag, AuraDamage.Debuff.DebuffFrequency);
		// Debuff Duration
		DebuffTag = GetDebuffDurationByType(DebuffType);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DebuffTag, AuraDamage.Debuff.DebuffDuration);
		// Debuff Should Hit React
		DebuffTag = GetDebuffShouldHitReactByType(DebuffType);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DebuffTag, AuraDamage.Debuff.bShouldHitReact);
	}

	// Apply the damage ability to the target actor
	Params.SourceAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data,
		Params.TargetAbilitySystemComponent);
	return ContextHandle;
}

void UAuraAbilitySystemLibrary::AssignAndApplyToSelfSetByCallerEffect(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpecHandle& EffectSpecHandle, const FGameplayTag& DataTag, const float Magnitude, const bool
                                                                      SupressWarnings)
{
	// Early check
	if (!AbilitySystemComponent || !EffectSpecHandle.IsValid())
	{
		return;
	}
	
	// Assign the set by SetByCallerMagnitude for the provided Tag
	EffectSpecHandle.Data->SetSetByCallerMagnitude(DataTag, Magnitude);
	
	// Fill out the Effect Spec with all other set by caller tags, so there are no warnings
	if (SupressWarnings)
	{
		// Early check
		const UGameplayEffect* GameplayEffect = EffectSpecHandle.Data->Def;
		if (!GameplayEffect)
		{
			return;
		}
		
		// Go through the modifiers to find the set by caller ones
		for (const FGameplayModifierInfo& ModifierInfo : GameplayEffect->Modifiers)
		{
			// Early check
			if (ModifierInfo.ModifierMagnitude.GetMagnitudeCalculationType() != EGameplayEffectMagnitudeCalculation::SetByCaller)
			{
				continue;
			}
			
			// If found, then pass the set by caller tag into the Effect Spec with a magnitude of 0.f
			// Note: Except for the one already applied
			const FGameplayTag SetByCallerTag = ModifierInfo.ModifierMagnitude.GetSetByCallerFloat().DataTag;
			if (SetByCallerTag.MatchesTagExact(DataTag))
			{
				continue;
			}
			
			EffectSpecHandle.Data->SetSetByCallerMagnitude(SetByCallerTag, 0.f);
		}
	}
	
	// Now apply the Effect Spec to Self
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	// Retrieve the Aura Gameplay Effect Context and check if it was a Blocked Hit
	const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	return AuraGameplayEffectContext ? AuraGameplayEffectContext->IsBlockedHit() : false; 
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	// Retrieve the Aura Gameplay Effect Context and check if it was a Critical Hit
	const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	return AuraGameplayEffectContext ? AuraGameplayEffectContext->IsCriticalHit() : false; 
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsBlockedHit)
{
	// Retrieve the Aura Gameplay Effect Context and set its Blocked Hit flag
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	const bool bInIsCriticalHit)
{
	// Retrieve the Aura Gameplay Effect Context and set its Critical Hit flag
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

bool UAuraAbilitySystemLibrary::IsDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	// Retrieve the Aura Gameplay Effect Context and check if it was a Critical Hit
	const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	return AuraGameplayEffectContext ? AuraGameplayEffectContext->IsDebuff() : false; 
}

void UAuraAbilitySystemLibrary::SetIsDebuff(FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsDebuff)
{
	// Retrieve the Aura Gameplay Effect Context and set its Critical Hit flag
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetIsDebuff(bInIsDebuff);
	}
}

FVector UAuraAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	// Retrieve the Aura Gameplay Effect Context and Get its death Impulse
	const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	return AuraGameplayEffectContext ? AuraGameplayEffectContext->GetDeathImpulse() : FVector::ZeroVector;
}

void UAuraAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle,
                                                const FVector& DeathImpulse)
{
	// Retrieve the Aura Gameplay Effect Context and set its DeathImpulse
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetDeathImpulse(DeathImpulse);
	}
}

FVector UAuraAbilitySystemLibrary::GetKnockback(const FGameplayEffectContextHandle& EffectContextHandle)
{
	// Retrieve the Aura Gameplay Effect Context and Get its knockback
	const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	return AuraGameplayEffectContext ? AuraGameplayEffectContext->GetKnockback() : FVector::ZeroVector;
}

void UAuraAbilitySystemLibrary::SetKnockback(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& Knockback)
{
	// Retrieve the Aura Gameplay Effect Context and set its Knockback
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetKnockback(Knockback);
	}
}

bool UAuraAbilitySystemLibrary::IsDebuffSuccessful(const FGameplayEffectContextHandle& EffectContextHandle,
                                                   const FGameplayTag& DamageType)
{
	// Retrieve the Aura Gameplay Effect Context and check if it was a Successful Debuff Application
	const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	return AuraGameplayEffectContext ? AuraGameplayEffectContext->IsSuccessfulDebuff(DamageType) : false;
}

void UAuraAbilitySystemLibrary::SetIsDebuffSuccessful(FGameplayEffectContextHandle& EffectContextHandle,
                                                      const FGameplayTag& DamageType, const bool bInDebuffSuccessful)
{
	// Retrieve the Aura Gameplay Effect Context and set its Successful Debuff flag
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetSuccessfulDebuff(DamageType, bInDebuffSuccessful);
	}
}

bool UAuraAbilitySystemLibrary::WasAnyDebuffApplied(const FGameplayEffectContextHandle& EffectContextHandle)
{
	// Retrieve the Aura Gameplay Effect Context and check if it was a Successful Debuff Application
	const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	return AuraGameplayEffectContext ? AuraGameplayEffectContext->WasAnyDebuffApplied() : false;
}

FAuraDebuff UAuraAbilitySystemLibrary::GetDebuff(const FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& DamageType)
{
	// Retrieve the Aura Gameplay Effect Context and check its Debuff Damage
	const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	return AuraGameplayEffectContext ? AuraGameplayEffectContext->GetDebuff(DamageType) : FAuraDebuff();
}

float UAuraAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& DamageType)
{
	// Retrieve the Aura Gameplay Effect Context and check its Debuff Damage
	const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	return AuraGameplayEffectContext ? AuraGameplayEffectContext->GetDebuffDamage(DamageType) : 0.f;
}

void UAuraAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle,
                                                const FGameplayTag& DamageType, const float InDebuffDamage)
{
	// Retrieve the Aura Gameplay Effect Context and set its Successful Debuff flag
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetDebuffDamage(DamageType, InDebuffDamage);
	}
}

float UAuraAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& DamageType)
{
	// Retrieve the Aura Gameplay Effect Context and check its Debuff Frequency
	const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	return AuraGameplayEffectContext ? AuraGameplayEffectContext->GetDebuffFrequency(DamageType) : 0.f;
}

void UAuraAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle,
                                                   const FGameplayTag& DamageType, const float InDebuffFrequency)
{
	// Retrieve the Aura Gameplay Effect Context and set its Successful Debuff flag
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetDebuffFrequency(DamageType, InDebuffFrequency);
	}
}

float UAuraAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& DamageType)
{
	// Retrieve the Aura Gameplay Effect Context and check its Debuff Duration
	const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	return AuraGameplayEffectContext ? AuraGameplayEffectContext->GetDebuffDuration(DamageType) : 0.f;
}

void UAuraAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle,
                                                  const FGameplayTag& DamageType, const float InDebuffDuration)
{
	// Retrieve the Aura Gameplay Effect Context and set its Successful Debuff flag
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetDebuffDuration(DamageType, InDebuffDuration);
	}
}

bool UAuraAbilitySystemLibrary::ShouldHitReact(const FGameplayEffectContextHandle& EffectContextHandle,
                                               const FGameplayTag& DamageType)
{
	// Retrieve the Aura Gameplay Effect Context and check if it should Hit React
	const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	return AuraGameplayEffectContext ? AuraGameplayEffectContext->ShouldHitReact(DamageType) : true;
}

bool UAuraAbilitySystemLibrary::ShouldAnyHitReact(const FGameplayEffectContextHandle& EffectContextHandle)
{
	// Retrieve the Aura Gameplay Effect Context and check if it should Hit React
	const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	return AuraGameplayEffectContext ? AuraGameplayEffectContext->ShouldHitReact() : true;
}

void UAuraAbilitySystemLibrary::SetShouldHitReact(FGameplayEffectContextHandle& EffectContextHandle,
                                                  const FGameplayTag& DamageType, const bool InShouldHitReact)
{
	// Retrieve the Aura Gameplay Effect Context and set its Successful Debuff flag
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetShouldHitReact(DamageType, InShouldHitReact);
	}
}

TArray<FGameplayTag> UAuraAbilitySystemLibrary::GetDamageTypes(
	const FGameplayEffectContextHandle& EffectContextHandle)
{
	// Retrieve the Aura Gameplay Effect Context and check its Debuff Type
	const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	return AuraGameplayEffectContext ? AuraGameplayEffectContext->GetDamageTypes() : TArray<FGameplayTag>();
}

void UAuraAbilitySystemLibrary::AddDamageType(FGameplayEffectContextHandle& EffectContextHandle,
                                              const FGameplayTag& DamageType)
{
	// Retrieve the Aura Gameplay Effect Context and set its Successful Debuff flag
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->AddDamageType(DamageType);
	}
}

void UAuraAbilitySystemLibrary::AddDamageTypeAndDebuff(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& DamageType, const FAuraDebuff& Debuff)
{
	// Retrieve the Aura Gameplay Effect Context and set its Successful Debuff flag
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->AddDamageTypeAndDebuff(DamageType, Debuff);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius(const UObject* WorldContextObject,
                                                          TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                          const FVector& OriginPoint, const bool DebugSphere /*= false*/)
{
	// Create the collision query parameters and signal the ignored actors
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	// Query scene for overlapping actors
	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(
			Overlaps, 
			OriginPoint, 
			FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), 
			FCollisionShape::MakeSphere(Radius), 
			SphereParams);
	
		// If there is a need to debug the testing region
		if (DebugSphere)
		{
			DrawDebugSphere(World, OriginPoint, Radius,12,FColor::Red,false, 3.f);
		}
	}
	
	// Register all overlapped actors on the out vector that:
	// - Implement the AuraCombatInterface
	// - Are NOT dead
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* OverlappedActor = Overlap.GetActor();
		if (!OverlappedActor)
		{
			continue;
		}
		
		// Check if it implements the AuraCombatInterface and is not dead
		if (OverlappedActor->Implements<UAuraCombatInterface>() && !IAuraCombatInterface::Execute_IsDead(OverlappedActor))
		{
			OutOverlappingActors.AddUnique(OverlappedActor);
		}
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	// If by any chance the same actor was supplied, then it is friendly
	if (FirstActor == SecondActor)
	{
		return false;
	}
	
	// Check if the actors are players or enemies
	const bool bBothArePlayers = FirstActor->ActorHasTag(ACTOR_PlAYER_TAG) && SecondActor->ActorHasTag(ACTOR_PlAYER_TAG);
	const bool bBothAreEnemies = FirstActor->ActorHasTag(ACTOR_ENEMY_TAG) && SecondActor->ActorHasTag(ACTOR_ENEMY_TAG);
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	
	// If the actors are not both players or both enemies, then they are not friends!
	// This encompasses the case where they both do not have tags (they will be considered hostile)
	return !bFriends;
}

FVector UAuraAbilitySystemLibrary::GetRandomLocationInLine(const FVector& Origin, const FVector& Direction,
                                                                const float MinDistance, const float MaxDistance)
{
	return Origin + Direction * FMath::RandRange(MinDistance, MaxDistance);
}

void UAuraAbilitySystemLibrary::CheckIsClassChildOf(const UClass* Parent, const UClass* Child)
{
	checkf(Parent, TEXT("CheckIsClassChildOf() called with a nullptr Parent class object."));
	checkf(Child, TEXT("CheckIsClassChildOf() called with a nullptr Child class object."));
	checkf(Child->IsChildOf(Parent), TEXT("CheckIsClassChildOf() called with invalid Child class, %s must be a child of %s."), *Child->GetName(), *Parent->GetName());
}

TArray<FRotator> UAuraAbilitySystemLibrary::EvenlySpacedRotators(const FVector& ForwardVector, const FVector& Axis,
                                                                 const float Spread, const int32 NumRotators)
{
	TArray<FRotator> Rotators;
	auto AddingRotators = [&](const FVector& Vector)
	{
		Rotators.Add(Vector.Rotation());
	};
	
	// Call the internal calculation method 
	InternalEvenlySpacedCalculation(AddingRotators, ForwardVector, Axis, Spread, NumRotators);
	
	return Rotators;
}

TArray<FVector> UAuraAbilitySystemLibrary::EvenlySpacedVectors(const FVector& ForwardVector, const FVector& Axis,
                                                               const float Spread, const int32 NumVectors)
{
	TArray<FVector> Vectors;
	auto AddingVector = [&](const FVector& Vector)
	{
		Vectors.Add(Vector);
	};
	
	// Call the internal calculation method 
	InternalEvenlySpacedCalculation(AddingVector, ForwardVector, Axis, Spread, NumVectors);
	
	return Vectors;
}

FWidgetControllerParams UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, AAuraHUD*& AuraHUD)
{
	// Assumes this method is called from an autonomous client (Player), 
	// and fetches the first available player controller
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PlayerController)
	{
		// No valid Player Controller 
		return	FWidgetControllerParams();
	}
	
	// Get the HUD to find the overlay controller
	AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD());
	if (!AuraHUD)
	{
		// No valid HUD was found
		return	FWidgetControllerParams();
	}

	// Get required data to initialize an Overlay Controller
	// Need to construct a FWidgetControllerParams (requirements):
	// - Player Controller
	// - Player State
	// - Ability System Component
	// - Attribute Set

	// Get Player State
	AAuraPlayerState* AuraPlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
	if (!AuraPlayerState)
	{
		// No player state was defined
		return	FWidgetControllerParams();
	}
	
	// Get Ability System Component and Attribute Set
	UAbilitySystemComponent* AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	UAttributeSet* AttributeSet = AuraPlayerState->GetAttributeSet();
	if (!AbilitySystemComponent || !AttributeSet)
	{
		// Either the ASC or the AS are not valid 
		return	FWidgetControllerParams();
	}
	
	// Make the Widget Controller params
	return FWidgetControllerParams(PlayerController, AuraPlayerState,AbilitySystemComponent, AttributeSet);
}

void UAuraAbilitySystemLibrary::InternalEvenlySpacedCalculation(const TFunction<void(FVector)>& Callback,
	const FVector& ForwardVector, const FVector& Axis, const float Spread, const int32 NumObjects)
{
	// Find vectors around an axis and spread from a forward vector
	const FVector LeftOfSpread = ForwardVector.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumObjects > 1)
	{
		const float DeltaSpread = Spread / (NumObjects - 1);
		for (int32 i = 0; i < NumObjects; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
			// Call the callback to act on this vector
			Callback(Direction);
		}
	}
	else
	{
		// Call the callback to act on this vector
		Callback(ForwardVector);
	}
}
