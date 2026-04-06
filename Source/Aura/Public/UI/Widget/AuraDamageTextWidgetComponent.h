

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "AuraDamageTextWidgetComponent.generated.h"

/**
 * Widget Component to display damage received
 */
UCLASS()
class AURA_API UAuraDamageTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	// Set damage text in the used widget
	// bBlockedHit and bCriticalHit will change the color of the text
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage, bool bBlockedHit, bool bCriticalHit);
};
