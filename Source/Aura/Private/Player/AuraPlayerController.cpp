


#include "Player/AuraPlayerController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTagsManager.h"
#include "Components/SplineComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/AuraEnemyInterface.h"


AAuraPlayerController::AAuraPlayerController()
{
	// Should replicate
	bReplicates = true;
	
	// Create Spline
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
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
	
	// Our input component must be of UAuraInputComponent type! (crash if failed)
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	
	/*
	 * Bindings
	 */
	// Bind move action
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	
	// Bind Ability Actions
	AuraInputComponent->BindAbilityActions(AuraInputConfigDataAsset,this, 
		&ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraAbilitySystemComponent()
{
	// If there's no valid AuraAbilitySystemComponent
	if (!AuraAbilitySystemComponent)
	{
		// Get it from the UAbilitySystemBlueprintLibrary using the pawn connected to this controller
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	// Do note that this can still return null
	return AuraAbilitySystemComponent;
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

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	// If the LMB was pressed (used for moving)
	if (InputTag.MatchesTagExact(FAuraGameplayTagsManager::Get().InputTag_LMB))
	{
		// If there is an object/actor under the cursor at the moment of pressing
		// Then we are targeting and not pointing to the environment
		bTargeting = CurrentActor ? true : false ;
	
		// A new possible destination was set so we should stop auto running
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(const FGameplayTag InputTag)
{
	// Early exit
	if (!GetAuraAbilitySystemComponent())
	{
		return;
		
	}
	
	// Warn the Ability System Component that this Input was released
	GetAuraAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
}

void AAuraPlayerController::AbilityInputTagHeld(const FGameplayTag InputTag)
{
	// If it is NOT the LMB being pressed (used for moving)
	// Then check for Abilities
	if (!InputTag.MatchesTagExact(FAuraGameplayTagsManager::Get().InputTag_LMB) || bTargeting)
	{
		// Get a valid Aura Ability System Component
		if (GetAuraAbilitySystemComponent())
		{
			// Warn the Ability System Component that this Input is being held
			// This will check if there are any abilities bound to it
			GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
			return;
		}
	}
	
	// If the LMB is being pressed and no object/actor is being targeted, 
	// then the character should be moving.
	
	// Update the follow time (how long has the character been moving - heading to destination)
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// Get the destination where the character should be moving
	// It will be the world position under the cursor
	FHitResult Hit;
	if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit))
	{
		CachedDestination = Hit.ImpactPoint;
	}
	
	// If controlling a pawn then move it to the destination
	// This will be done via adding movement and not setting location
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// Get the direction of the movement (destination position - current position),
		// then normalise it for a unitary vector with the direction
		const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		
		// Add a singular movement input into the direction of the destination
		ControlledPawn->AddMovementInput(WorldDirection);
	}
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
