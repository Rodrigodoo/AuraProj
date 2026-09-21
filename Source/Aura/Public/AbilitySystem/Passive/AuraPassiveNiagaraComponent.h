

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "AuraPassiveNiagaraComponent.generated.h"

/**
 * Niagara Component to display effects related to Passive Abilities
 */
UCLASS()
class AURA_API UAuraPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
	
public:
	UAuraPassiveNiagaraComponent();
	
	// Tag to identify the associate passive spell
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Abilities.Passive"))
	FGameplayTag PassiveSpellTag;
	
protected:
	virtual void BeginPlay() override;
	
	// Method to be called whenever a passive ability status changes
	void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);
	
};
