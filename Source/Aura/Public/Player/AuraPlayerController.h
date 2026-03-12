

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"


class UAuraAbilitySystemComponent;
class IAuraEnemyInterface;
class UAuraInputConfigDataAsset;
class UInputAction;
class UInputMappingContext;
struct FGameplayTag;
struct FInputActionValue;

/**
 * Player controller for aura characters
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();
	
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
	//~ Begin - Ability System Component
	// Ability System Component connected to this controller
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	
	// Retrieves the Aura Ability system component connected to this controller
	UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent();
	//~ End - Ability System Component
	
	//~ Begin - Track Enemies
	
	// Last Enemy
	TScriptInterface<IAuraEnemyInterface> LastActor;
	// Current Enemy
	TScriptInterface<IAuraEnemyInterface> CurrentActor;
	//~ End - Track Enemies
	
	//~ Begin - Input and callbacks
	
	// Aura Input Mapping Context
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	
	// Move Action Input
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	// Move the character 2D (XY)
	void Move(const FInputActionValue& InputActionValue);
	
	// Callback method to deal with input pressed
	void AbilityInputTagPressed(FGameplayTag InputTag);
	// Callback method to deal with input released
	void AbilityInputTagReleased(FGameplayTag InputTag);
	// Callback method to deal with input held
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	// Input Config Data Asset that maps gameplay tags to actions
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfigDataAsset> AuraInputConfigDataAsset;
	//~ End - Input and callbacks

	// Traces objects under cursor
	void CursorTrace();
};
