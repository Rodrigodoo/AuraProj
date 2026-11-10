


#include "UI/ViewModel/MVVM_LoadMenu.h"

#include "Game/AuraGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadMenu::InitializeLoadSlots()
{
	// Create the Load Slot View Models
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	
	// Initialize Slot Names
	LoadSlot_0->LoadSlotName = FString("LoadSlot_0");
	LoadSlot_1->LoadSlotName = FString("LoadSlot_1");
	LoadSlot_2->LoadSlotName = FString("LoadSlot_2");
	
	// Add the Load Slot View Models to the Map
	LoadSlots.Add(0,LoadSlot_0);
	LoadSlots.Add(1,LoadSlot_1);
	LoadSlots.Add(2,LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadMenu::GetLoadSlotByIndex(const int Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadMenu::NewSlotButtonPressed(int32 SlotIndex, const FString& EnteredName)
{
	 UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	 if (!AuraGameInstance)
	 {
	 	// Early Check
		return; 
	 }
	
	// Set the relevant data to the Load Slot
	UMVVM_LoadSlot* LoadSlot = LoadSlots.FindChecked(SlotIndex);
	LoadSlot->PlayerName = EnteredName;
	
	// Save the Data to Disk
	AuraGameInstance->SaveSlotData(LoadSlot);
	
	// Initialize this slot
	LoadSlot->InitializeSlot();
}

void UMVVM_LoadMenu::NewGameButtonPressed(int32 SlotIndex)
{
	// Broadcast to change to the next widget
	LoadSlots.FindChecked(SlotIndex)->SetWidgetSwitcherIndexDelegate.Broadcast(1);
}

void UMVVM_LoadMenu::SelectSlotButtonPressed(int32 SlotIndex)
{
}
