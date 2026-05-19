

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraHUD.generated.h"

class UAuraSpellMenuController;
class UAuraAttributeMenuController;
class UAttributeSet;
class UAbilitySystemComponent;
class UAuraOverlayController;
class UAuraUserWidget;
struct FWidgetControllerParams;

/**
 * HUD to be used by clients in the project
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	// Get the Overlay Controller if it doesn't exist makes one
	// Note: - Works like a singleton.
	// - Initialises the Controller (Sets up controller parameters and Binds delegates)
	UAuraOverlayController* GetAuraOverlayController(const FWidgetControllerParams& WCParams);
	
	// Get the Attribute Menu Controller if it doesn't exist makes one
	// Note: - Works like a singleton.
	// - Initialise Controller (Sets up controller parameters and Binds delegates)
	UAuraAttributeMenuController* GetAuraAttributeMenuController(const FWidgetControllerParams& WCParams);
	
	// Get the Spell Menu Controller if it doesn't exist makes one
	// Note: - Works like a singleton.
	// - Initialise Controller (Sets up controller parameters and Binds delegates)
	UAuraSpellMenuController* GetAuraSpellMenuController(const FWidgetControllerParams& WCParams);
	
	// Initialise the Overlay.
	// - Construct the OverlayController
	// - Construct the OverlayWidget
	// - Set the OverlayController in the OverlayWidget
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	// Gets the Controller, if it doesn't exist makes one
	// Note: - Works like a singleton.
	// - Initialises the Controller (Sets up controller parameters and Binds delegates)
	template <typename T = UAuraWidgetController>
	T* CreateOrGetWidgetController(TObjectPtr<T>& WidgetController,
											 const TSubclassOf<UAuraWidgetController>& WidgetControllerClass,
											 const FWidgetControllerParams& WCParams);

	// Overlay Widget
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	// Overlay Widget Class
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
	
	//~ Begin - Controllers
	
	// Overlay Controller
	UPROPERTY()
	TObjectPtr<UAuraOverlayController> OverlayController;
	
	// Overlay Controller Class
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraOverlayController> OverlayControllerClass;
	
	// Attribute Menu Controller
	UPROPERTY()
	TObjectPtr<UAuraAttributeMenuController> AttributeMenuController;
	
	// Attribute Menu Controller Class
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraAttributeMenuController> AttributeMenuControllerClass;
	
	// Spell Menu Controller
	UPROPERTY()
	TObjectPtr<UAuraSpellMenuController> SpellMenuController;
	
	// Spell Menu Controller Class
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraSpellMenuController> SpellMenuControllerClass;
	//~ End - Controllers
};

template <typename T>
T* AAuraHUD::CreateOrGetWidgetController(TObjectPtr<T>& WidgetController,
	const TSubclassOf<UAuraWidgetController>& WidgetControllerClass, const FWidgetControllerParams& WCParams)
{
	// Checks if the Widget Controller is a child of UAuraWidgetController, otherwise it will crash
	UAuraAbilitySystemLibrary::CheckIsClassChildOf(UAuraWidgetController::StaticClass(), T::StaticClass());
	
	// If there is no widget controller, then make a new one
	// And call its SetWidgetControllerParams and BindCallbacksToDependencies
	if (!WidgetController)
	{
		WidgetController = NewObject<T>(this, WidgetControllerClass);
		WidgetController->SetWidgetControllerParams(WCParams);
		WidgetController->BindCallbacksToDependencies();
	}
 
	return WidgetController;
}
