

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
	
	// Initializes the Gameplay Tags for this project
	// Enrolls them in the UGameplayTagManager (Unreal Version)
	static void InitializeNativeGameplayTags();
	
protected:
	
private:
	static FAuraGameplayTagsManager Instance;
	
};