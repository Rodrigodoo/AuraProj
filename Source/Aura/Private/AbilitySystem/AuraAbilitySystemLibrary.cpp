


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Data/AuraCharacterClassInfoDataAsset.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UAuraOverlayController* UAuraAbilitySystemLibrary::GetOverlayController(const UObject* WorldContextObject)
{
	
	// Assumes this method is called from an autonomous client (Player), 
	// and fetches the first available player controller
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PlayerController)
	{
		// No valid Player Controller 
		return	nullptr;
	}

	// Get the HUD to find the overlay controller
	AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD());
	if (!AuraHUD)
	{
		// No valid HUD was found
		return	nullptr;
	}
	
	// Get required data to initialize a Overlay Controller
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
		return	nullptr;
	}
	
	// Get Ability System Component and Attribute Set
	UAbilitySystemComponent* AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	UAttributeSet* AttributeSet = AuraPlayerState->GetAttributeSet();
	if (!AbilitySystemComponent || !AttributeSet)
	{
		// Either the ASC or the AS are not valid 
		return	nullptr;
	}
	
	// Make the Widget Controller params
	const FWidgetControllerParams ControllerParams(PlayerController, AuraPlayerState,AbilitySystemComponent, AttributeSet);
	
	// Retrieve the Overlay Controller (If one does not exist it will create one)
	return AuraHUD->GetAuraOverlayController(ControllerParams);
	
}

UAuraAttributeMenuController* UAuraAbilitySystemLibrary::GetAttributeMenuController(const UObject* WorldContextObject)
{
	// Assumes this method is called from an autonomous client (Player), 
	// and fetches the first available player controller
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PlayerController)
	{
		// No valid Player Controller 
		return	nullptr;
	}

	// Get the HUD to find the overlay controller
	AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD());
	if (!AuraHUD)
	{
		// No valid HUD was found
		return	nullptr;
	}
	
	// Get required data to initialize a Overlay Controller
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
		return	nullptr;
	}
	
	// Get Ability System Component and Attribute Set
	UAbilitySystemComponent* AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	UAttributeSet* AttributeSet = AuraPlayerState->GetAttributeSet();
	if (!AbilitySystemComponent || !AttributeSet)
	{
		// Either the ASC or the AS are not valid 
		return	nullptr;
	}
	
	// Make the Widget Controller params
	const FWidgetControllerParams ControllerParams(PlayerController, AuraPlayerState,AbilitySystemComponent, AttributeSet);
	
	return AuraHUD->GetAuraAttributeMenuController(ControllerParams);
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, const EAuraCharacterClass CharacterClass, float Level, UAbilitySystemComponent*
                                                            AbilitySystemComponent)
{
	// Get the game mode and the Character Class Info Data Asset
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!AuraGameMode)
	{
		// If this is called on the client it will fail since GameMode is only available on the server
		return;
	}
	
	const UAuraCharacterClassInfoDataAsset* CharacterClassInfoDataAsset = AuraGameMode->CharacterClassInfoDataAsset;
	check(CharacterClassInfoDataAsset)
	
	// Retrieve the Character Class Info for this specific RPG Class
	const auto [PrimaryAttributes] = CharacterClassInfoDataAsset->GetClassDefaultInfo(CharacterClass);
	
	// Apply the Gameplay Effects to the provided Ability System Component
	
	// Get the Avatar actor to make as source Object
	const AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor(); 
	
	//~ Primary Attributes
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		PrimaryAttributes, 
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
	UAbilitySystemComponent* AbilitySystemComponent)
{
	// Get the game mode and the Character Class Info Data Asset
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!AuraGameMode)
	{
		// If this is called on the client it will fail since GameMode is only available on the server
		return;
	}
	
	const UAuraCharacterClassInfoDataAsset* CharacterClassInfoDataAsset = AuraGameMode->CharacterClassInfoDataAsset;
	check(CharacterClassInfoDataAsset)
	
	// Loop  through the common abilities and apply them to the Ability System Component's owner
	for (const TSubclassOf<UGameplayAbility> Ability : CharacterClassInfoDataAsset->CommonAbilities)
	{
		// Create an Ability Spec for this ability
		FGameplayAbilitySpec AbilitySpec(Ability, 1);
		
		// Grant the ability
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}
