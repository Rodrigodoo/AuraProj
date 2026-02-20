

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UAuraOverlayWidgetController;
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
	
	// Get the Overlay Widget Controller if it doesn't exist makes one
	// Note: - Works like a singleton.
	// - Initialise Controller (Sets up controller parameters and Binds delegates)
	UAuraOverlayWidgetController* GetAuraOverlayWidgetController(const FWidgetControllerParams& WCParams);
	
	// Initialise the Overlay.
	// - Construct the OverlayWidgetController
	// - Construct the OverlayWidget
	// - Set the OverlayWidgetController in the OverlayWidget
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
private:

	// Overlay Widget Class
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
	
	//~ Begin Controller
	
	// Overlay Widget Controller
	UPROPERTY()
	TObjectPtr<UAuraOverlayWidgetController> OverlayWidgetController;
	
	// Overlay Widget Controller Class
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraOverlayWidgetController> OverlayWidgetControllerClass;
	//~ End Controller
};
