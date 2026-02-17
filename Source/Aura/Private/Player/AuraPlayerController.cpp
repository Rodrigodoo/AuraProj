


#include "Player/AuraPlayerController.h"
#include "EnhancedInputComponent.h"
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

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// Cast to UEnhancedInputComponent (crash if failed)
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	// Bind input action
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// Move the character
	
	// Early exit checks
	APawn* ControlledPawn = GetPawn<APawn>();
	if (!ControlledPawn)
	{
		// If not pawn is controlled no need to move
		return;
	}
	
	// Get input as 2D Vector
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	
	// Get Current CurrentControlRotationYaw (initialize Pitch and Roll to 0.0f)
	const FRotator CurrentControlRotation = GetControlRotation();
	const FRotator CurrentControlRotationYaw = FRotator(0.0f, CurrentControlRotation.Yaw, 0.0f);
	
	// Find Forward Direction and Right Direction
	const FVector ForwardDirection = FRotationMatrix(CurrentControlRotationYaw).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(CurrentControlRotationYaw).GetUnitAxis(EAxis::Y);
	
	// Move Pawn 
	// Note: Forward Direction was setup to the Y axis [W/S]
	// Right Direction was setup to the X axis [A/D]
	ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
	ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
}
