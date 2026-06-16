

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "AuraDebuffNiagaraComponent.generated.h"

/**
 * Niagara Effect to be played whenever a Debuff is applied to a character
 */
UCLASS()
class AURA_API UAuraDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UAuraDebuffNiagaraComponent();
	
	// Id Tag (when receives the tag, activate, when the owner loses the tag, Deactivate)
	UPROPERTY(VisibleAnywhere, meta = (Categories = "Debuff"))
	FGameplayTag DebuffTag;
	
protected:
	virtual void BeginPlay() override;
	
	// Debuff tag changed callback
	void DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	// Process the Owner's Death
	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);
};
