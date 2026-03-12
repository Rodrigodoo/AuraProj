


#include "Player/AuraPlayerController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTagsManager.h"
#include "Components/SplineComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
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
	
	// Perform Auto Running if required
	AutoRun();
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
	// If it is NOT the LMB being pressed (used for moving)
	// Then check for Abilities
	if (!InputTag.MatchesTagExact(FAuraGameplayTagsManager::Get().InputTag_LMB) || bTargeting)
	{
		// Get a valid Aura Ability System Component
		if (GetAuraAbilitySystemComponent())
		{
			// Warn the Ability System Component that this Input was released
			// This will check if there are any abilities bound to it
			GetAuraAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
			return;
		}
	}
	
	// If the LMB is being pressed and no object/actor is being targeted, 
	// then the character should be moving.
	
	// If this was a short press (FollowTime less than the threshold), then create a navigation path for the character to auto run
	const APawn* ControlledPawn = GetPawn<APawn>(); 
	if (ControlledPawn && FollowTime <= ShortPressThreshold)
	{
		// Find the path to the location synchronously, which should find all obstacles between 
		// the character and the Cached Destination.
		// It will create navigation point for the character to follow.
		if (UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(
			this, ControlledPawn->GetActorLocation(),CachedDestination))
		{
			// Add the navigation point to the Spline
			
			// Start by clearing any existing points
			Spline->ClearSplinePoints();
			
			// Go through all the path points and add them to the Spline
			for (const FVector& PathPointLocation :  NavigationPath->PathPoints)
			{
				// Add them in World coordinates (could call AddSplineWorldPoint but this way we have one less call)
				Spline->AddSplinePoint(PathPointLocation, ESplineCoordinateSpace::World);
			}
			
			// Since the CachedDestination might not be on a valid point on the NavMesh 
			// the character might never reach it and stop running.
			// Therefore, need to update the CachedDestination to the last valid point on the Navigation Path/Spline
			CachedDestination = NavigationPath->PathPoints[NavigationPath->PathPoints.Num() - 1];
			
			// Flag that the character is now auto running
			bAutoRunning = true;
		}
	}
	
	// Reset follow time and targeting
	FollowTime = 0.f;
	bTargeting = false;
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
	if (CursorHit.bBlockingHit)
	{
		CachedDestination = CursorHit.ImpactPoint;
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

void AAuraPlayerController::AutoRun()
{
	// If we are not auto running return early
	if (!bAutoRunning)
	{
		return;
	}
	
	// If there is a Controlled Pawn move it towards the next point in the Spline with each tick
	APawn* ControlledPawn = GetPawn<APawn>();
	if (!IsValid(ControlledPawn))
	{
		return;
	}
	
	// Find the location on the spline closest to the character
	// Due to the nature of the movement the controlled character might not be in the exact spline location
	const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		
	// Retrieve the tangential direction of the spline at the closest point to the character
	// The direction should follow the spline general direction (towards the destination)
	const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		
	// With the direction of the Spline, add a singular movement input to the character towards that direction
	ControlledPawn->AddMovementInput(Direction);
		
	// Now check if the character is within the acceptance radius of the destination 
	// If so, it should stop moving (Arrived)
		
	// Check the distance from the spline point to the destination by getting the length of the vector between them 
	const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
	if (DistanceToDestination <= AutoRunAcceptanceRadius)
	{
		bAutoRunning = false;
	}
}

void AAuraPlayerController::CursorTrace()
{
	// Get hit result under the cursor
	GetHitResultUnderCursor(ECC_Visibility,false,CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		// No hit found
		return;
	}
	
	// Replace Current and Last Actor implementing IAuraEnemyInterface
	LastActor = CurrentActor;
	CurrentActor = CursorHit.GetActor();
	
	// Only need to change highlighting if the actors are different
	if (LastActor != CurrentActor)
	{
		// If Last Actor is valid UnHighlight it
		if (LastActor)
		{
			LastActor->UnHighlightActor();
		}
		
		// If Current Actor is valid Highlight it
		if (CurrentActor)
		{
			CurrentActor->HighlightActor();
		}
	}
}
