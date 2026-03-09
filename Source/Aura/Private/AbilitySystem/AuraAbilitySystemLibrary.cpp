


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

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
