

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadMenu.generated.h"

class UMVVM_LoadSlot;
/**
 * Load Menu Base Model-View-ViewModel class
 */
UCLASS()
class AURA_API UMVVM_LoadMenu : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	// Class of MVVM to use when creating View Models
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;
	
	// Initializes the Load Slots View Models
	void InitializeLoadSlots();
	
	// Retrieves the Load Slot View Model by index
	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotByIndex(const int Index) const;
	
private:
	// Map to track all the existing Model Views for the Load Slots
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;
	
	// Pointer for the 0 Load Slot
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;
	// Pointer for the 1 Load Slot
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;
	// Pointer for the 2 Load Slot
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;
};
