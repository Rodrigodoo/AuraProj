

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraLoadMenuWidget.generated.h"

/**
 * Base Widget class to be used by the Widgets in the Load Menu
 */
UCLASS()
class AURA_API UAuraLoadMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Method to Initialize this Widget with relevant data
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintInitializeWidget();
};
