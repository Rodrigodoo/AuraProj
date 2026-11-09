


#include "UI/HUD/AuraLoadMenuHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/MVVM_LoadMenu.h"
#include "UI/Widget/AuraLoadMenuWidget.h"

void AAuraLoadMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	
	// Create the View Model to connect to Load Menu Widget
	LoadMenuViewModel = NewObject<UMVVM_LoadMenu>(this,LoadMenuViewModelClass);
	
	// Create the Load menu widget to display
	LoadMenuWidget = CreateWidget<UAuraLoadMenuWidget>(GetWorld(), LoadMenuWidgetClass);
	check(LoadMenuWidget);
	
	// Add to the screen
	LoadMenuWidget->AddToViewport();
}
