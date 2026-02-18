

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;

/**
 * Aura's Player state in charge of controlling several of the players information like its AbilitySystemComponent
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AAuraPlayerState();
	
	// Returns the ability system component to use for this actor.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	// Returns the attribute set to use for this actor.
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	
protected:
	// Pointer to the PlayerState's Ability System Component (If there is one)
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	// Pointer to the PlayerState's Attribute Set (If there is one)
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
