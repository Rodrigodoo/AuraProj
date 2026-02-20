

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"


class IAuraEnemyInterface;
class UInputAction;
class UInputMappingContext;
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
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	// Move the character 2D (XY)
	void Move(const FInputActionValue& InputActionValue);
	
	// Traces objects under cursor
	void CursorTrace();
	
	// Track Enemies
	
	// Last Enemy
	TScriptInterface<IAuraEnemyInterface> LastActor;
	// Current Enemy
	TScriptInterface<IAuraEnemyInterface> CurrentActor;
};
