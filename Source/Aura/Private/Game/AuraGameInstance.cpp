


#include "Game/AuraGameInstance.h"

#include "Game/AuraLoadMenuSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UAuraGameInstance::SaveSlotData(UMVVM_LoadSlot* LoadSlot)
{
	// Early check
	if (!LoadSlot)
	{
		return;
	}
	// Note: User index should be something like a Steam ID
	// Therefore we are always checking the save objects by name
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->LoadSlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->LoadSlotName, 0);
	}
	
	USaveGame* SavedGameObject = UGameplayStatics::CreateSaveGameObject(LoadMenuSaveGameClass);
	UAuraLoadMenuSaveGame* LoadMenuSaveGame = Cast<UAuraLoadMenuSaveGame>(SavedGameObject);
	if (!LoadMenuSaveGame)
	{
		// Early check
		return;
	}
	
	// Record the relevant data
	LoadMenuSaveGame->PlayerName = LoadSlot->PlayerName;
	
	// Save the Load Menu Save Object
	UGameplayStatics::SaveGameToSlot(LoadMenuSaveGame, LoadSlot->LoadSlotName, 0);
}
