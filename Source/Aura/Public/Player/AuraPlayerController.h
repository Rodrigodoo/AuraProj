

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"


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
	
	
	// Traces objects under cursor
	void CursorTrace();
	
	//~ Begin - Track Enemies
	
	// Last Enemy
	TScriptInterface<IAuraEnemyInterface> LastActor;
	// Current Enemy
	TScriptInterface<IAuraEnemyInterface> CurrentActor;
	//~ End - Track Enemies
	
	//~ Begin - Input and callbacks
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	
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
};
