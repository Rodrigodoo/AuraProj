

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

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
	// Overlay Widget
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	// Get the Overlay Controller if it doesn't exist makes one
	// Note: - Works like a singleton.
	// - Initialise Controller (Sets up controller parameters and Binds delegates)
	UAuraOverlayController* GetAuraOverlayController(const FWidgetControllerParams& WCParams);
	
	// Initialise the Overlay.
	// - Construct the OverlayController
	// - Construct the OverlayWidget
	// - Set the OverlayController in the OverlayWidget
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
private:

	// Overlay Widget Class
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
	
	//~ Begin Controller
	
	// Overlay Controller
	UPROPERTY()
	TObjectPtr<UAuraOverlayController> OverlayController;
	
	// Overlay Widget Controller Class
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraOverlayController> OverlayControllerClass;
	//~ End Controller
};
