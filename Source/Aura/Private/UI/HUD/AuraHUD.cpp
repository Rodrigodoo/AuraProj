


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "Player/AuraPlayerState.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AuraOverlayWidgetController.h"

UAuraOverlayWidgetController* AAuraHUD::GetAuraOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	// If none exists create a new one
	if (!OverlayWidgetController)
	{
		// Creates Widget Controller and initialises its params
		OverlayWidgetController = NewObject<UAuraOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
	UAttributeSet* AS)
{
	// Early checks
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialised, please define in BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialised, please define in BP_AuraHUD"))
	
	// Create the Overlay Widget Controller
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UAuraOverlayWidgetController* WidgetController = GetAuraOverlayWidgetController(WidgetControllerParams);
	
	// Create the Overlay Widget 
	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->SetWidgetController(WidgetController);
	
	// Since the Widget Controller is added to the widget and all model data was initiated (PC, PS, etc...)
	// Can start binding/broadcast.
	WidgetController->BroadcastInitialValues();
	
	// Add to Viewport
	OverlayWidget->AddToViewport();
}
