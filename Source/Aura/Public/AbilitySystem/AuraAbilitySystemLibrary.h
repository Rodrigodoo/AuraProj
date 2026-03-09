

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAuraOverlayController;
/**
 * Library in charge of auxiliary methods for the Aura Ability System
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	// Get the Overlay Controller
	// Assumes it's called from an autonomous client
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemController|WidgetController")
	static UAuraOverlayController* GetOverlayController(const UObject* WorldContextObject);
};
