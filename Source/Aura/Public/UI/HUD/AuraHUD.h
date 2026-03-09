

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

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
	// - Initialise Controller (Sets up controller parameters and Binds delegates)
	UAuraOverlayController* GetAuraOverlayController(const FWidgetControllerParams& WCParams);
	
	// Get the Attribute Menu Controller if it doesn't exist makes one
	// Note: - Works like a singleton.
	// - Initialise Controller (Sets up controller parameters and Binds delegates)
	UAuraAttributeMenuController* GetAuraAttributeMenuController(const FWidgetControllerParams& WCParams);
	
	// Initialise the Overlay.
	// - Construct the OverlayController
	// - Construct the OverlayWidget
	// - Set the OverlayController in the OverlayWidget
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
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
	//~ End - Controllers
};
