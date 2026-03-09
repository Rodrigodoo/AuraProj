

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Contains and manages all the Aura project's Gameplay Tags (Singleton)
 */
struct FAuraGameplayTagsManager
{
public:
	// Returns the Instance of this manager
	const static FAuraGameplayTagsManager& Get() { return Instance; };
	
	// Initialises the Gameplay Tags for this project
	// Enrolls them in the UGameplayTagManager (Unreal Version)
	static void InitializeNativeGameplayTags();
	
	//~ Begin - Internal references to Gameplay Tags
	
	// Attributes.Secondary.Armor
	FGameplayTag Attributes_Secondary_Armor;
	
	//~ End - Internal references to Gameplay Tags 
protected:
	
private:
	// This Manager's Instance
	static FAuraGameplayTagsManager Instance;
	
};