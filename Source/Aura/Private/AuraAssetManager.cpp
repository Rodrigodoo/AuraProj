


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTagsManager.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	// Check the validity of the Engine
	check(GEngine);
	
	// Retrieve the Engine's Asset manager and cast it to this project's version
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	// Initialize the project's Gameplay Tags
	FAuraGameplayTagsManager::InitializeNativeGameplayTags();
} 
