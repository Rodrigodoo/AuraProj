

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * Aura project's Asset Manager.
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	// Gets the instance of this project's Asset Manager
	static UAuraAssetManager& Get();
	
protected:
	//~ Begin - UAssetManager Overrides
	
	// Starts initial load, gets called from InitializeObjectReferences
	virtual void StartInitialLoading() override;
	//~ End - UAssetManager Overrides
	
};
