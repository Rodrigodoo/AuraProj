


#include "Player/AuraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/AuraEnemyInterface.h"


AAuraPlayerController::AAuraPlayerController()
{
	// Should replicate
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	// Find object under cursor
	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// Input mapping context
	
	// Assert AuraContext
	check(AuraContext);
	
	// UEnhancedInputLocalPlayerSubsystem deals with adding mapping contexts
	// Since in multiplayer clients might not have Local Player if they are not the owning one
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(AuraContext,0);
	}

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

void AAuraPlayerController::CursorTrace()
{
	// Get hit result under the cursor
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility,false,CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		// No hit found
		return;
	}
	
	// Replace Current and Last Actor implementing IAuraEnemyInterface
	LastActor = CurrentActor;
	CurrentActor = CursorHit.GetActor();

	/** -----!Highlight Logic!-----
	 * Line Trace from cursor. There are several scenarios:
	 *	A. LastActor = null & CurrentActor = null
	 *		- Do nothing
	 *	B. LastActor = null & CurrentActor = valid
	 *		- Highlight the CurrentActor
	 *	C. LastActor = valid & CurrentActor = null
	 *		- Unhighlight the LastActor
	 *	D. LastActor = valid & CurrentActor = valid & LastActor != CurrentActor
	 *		- Unhighlight the LastActor & Highlight the CurrentActor
	 *	E. LastActor = valid & CurrentActor = valid & LastActor == CurrentActor
	 *		- Do nothing
	 */
	if (!LastActor)
	{
		if (CurrentActor)
		{
			// Case B
			CurrentActor->HighlightActor();
		}
		else
		{
			// Case A - Do Nothing
		}
	}
	else // LastActor is valid
	{
		if (!CurrentActor)
		{
			// Case C
			LastActor->UnHighlightActor();
		}
		else if (LastActor != CurrentActor)
		{
			// Case D
			LastActor->UnHighlightActor();
			CurrentActor->HighlightActor();
		}
		else
		{
			// Case E - do nothing
		}
	}
}
