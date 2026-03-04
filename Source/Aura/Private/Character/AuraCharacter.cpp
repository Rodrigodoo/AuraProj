


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	// Top down type character setup
	
	// Setup Character orientation
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, YawRotationRate, 0.0f);
	
	// Constrain Character movement to XY and snap to plane
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	// Character should not use Controllers rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AAuraCharacter::InitCharacterAndComponents()
{
	// Retrieve the Player state
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	// Initialises the Ability Actor Info
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
		AuraPlayerState->GetAbilitySystemComponent()))
	{
		// Inform the ASC that its AbilityActorInfo was set
		// This will bind delegates
		AuraAbilitySystemComponent->AbilityActorInfoSet();
	}
	
	// Pass references this character of the PlayerState's AbilitySystemComponent and AttributeSet
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	
	// Initialize Attribute values
	InitializePrimaryAttributes();
	
	// Initialise the HUD since we have all required information
	// Only pass check in Server or Owning client (other clients will fail check)
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* HUD = AuraPlayerController->GetHUD<AAuraHUD>())
		{
			// Initialise the HUD
			HUD->InitOverlay(AuraPlayerController, AuraPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		// Initialise and cache the Ability Actor Info on the server as it will have all the information needed.
		InitCharacterAndComponents();
	}
}

void AAuraCharacter::OnRep_PlayerState()
{
	// Only on clients
	Super::OnRep_PlayerState();
	
	// Initialise and cache the Ability Actor Info on the client as it will have all the information needed.
	InitCharacterAndComponents();
}
