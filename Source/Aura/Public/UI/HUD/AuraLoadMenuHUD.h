

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraLoadMenuHUD.generated.h"

class UMVVM_LoadMenu;
class UAuraLoadMenuWidget;
/**
 * HUD to be used in the Load Menu level
 */
UCLASS()
class AURA_API AAuraLoadMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	// Class of the Load Menu Widget we want to display
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadMenuWidgetClass;
	
	// Load Menu Widget to display on the HUD
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAuraLoadMenuWidget> LoadMenuWidget;
	
	// Class of the View Model we need to use to connect to the Menu Widget
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadMenu> LoadMenuViewModelClass;
	
	// View Model to connecto to the Load Menu Widget
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_LoadMenu> LoadMenuViewModel;
protected:
	virtual void BeginPlay() override;
};
