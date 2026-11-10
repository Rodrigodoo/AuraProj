

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndexSignature, int32, WidgetSwitcherIndex);

/**
 * Load Slot Base Model-View-ViewModel class
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	// Initializes the Slot
	void InitializeSlot();
	
	// The Player Name for this Slot
	UPROPERTY()
	FString PlayerName;
	
	// The Slot Name for this Slot
	UPROPERTY()
	FString LoadSlotName;
	
	// The Load Slot Index for this Slot
	UPROPERTY()
	int32 LoadSlotIndex;
	
	// Delegate to set Widget Switcher Index 
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndexSignature SetWidgetSwitcherIndexDelegate;
};
