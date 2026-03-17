

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AuraTargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * Class to retrieve the Target Data under the cursor (Mouse)
 */
UCLASS()
class AURA_API UAuraTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	// Factory method to create UAuraTargetDataUnderMouse objects
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta= (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAuraTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);
	
	// Output pin for when there is valid data under the mouse
	// Broadcasts a Target Data Handle (this one take a Target Hit)
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;
	
private:
	//~ Begin - UGameplayTask overrides
	
	// Called to trigger the actual task once the delegates have been set up 
	virtual void Activate() override;
	//~ End - UGameplayTask overrides
	
	// Send the mouse cursor data to the Server
	// Note: only use this on the client side
	void SendMouseCursorData();

};
