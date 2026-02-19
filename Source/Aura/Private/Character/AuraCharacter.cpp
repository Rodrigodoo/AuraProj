


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"

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

void AAuraCharacter::InitAndCacheAbilitySystemComponentAndAttributeSet(AAuraPlayerState* AuraPlayerState)
{
	// An invalid pointer was provided
	if (!AuraPlayerState)
	{
		return;
	}
	
	// Initialises the Ability Actor Info
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	
	// Pass references this character of the PlayerState's AbilitySystemComponent and AttributeSet
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		// Initialise and cache the Ability Actor Info on the server as it will have all the information needed.
		AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
		check(AuraPlayerState);
		InitAndCacheAbilitySystemComponentAndAttributeSet(AuraPlayerState);
	}
}

void AAuraCharacter::OnRep_PlayerState()
{
	// Only on clients
	Super::OnRep_PlayerState();
	
	// Initialise and cache the Ability Actor Info on the client as it will have all the information needed.
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	InitAndCacheAbilitySystemComponentAndAttributeSet(AuraPlayerState);
	
}
