


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"


AAuraPlayerController::AAuraPlayerController()
{
	// Should replicate
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// Input mapping context
	
	// Assert AuraContext
	check(AuraContext);
	
	// UEnhancedInputLocalPlayerSubsystem deals with adding mapping contexts
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext,0);
	
	// Mouse Cursor
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	// Input Mode
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}
