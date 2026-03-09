

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraAttributeMenuController.generated.h"

/**
 * Attribute Menu Controller responsible for controlling the Aura Attribute Menu Widget
 * BlueprintType - Can be used as type in BP Event Graphs
 * Blueprintable - Can make a blueprint based on this class
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAuraAttributeMenuController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	
	//~ Begin - UAuraWidgetController Overrides
	// Call delegates to broadcast initial values
	virtual void BroadcastInitialValues() override;
	
	// Binds callbacks to dependencies of this controller
	virtual void BindCallbacksToDependencies() override;
	//~ End - UAuraWidgetController Overrides
	
};
