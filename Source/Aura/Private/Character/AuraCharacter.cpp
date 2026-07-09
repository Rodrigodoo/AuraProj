


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTagsManager.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	// Top down type character setup
	
	// Setup Camera & Spring Arm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bEnableCameraLag = true;
	
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;
	
	// Level up Niagara component setup
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
	
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
	
	// Set aura as an Elementalist by default
	CharacterClass = EAuraCharacterClass::Elementalist;
}

int32 AAuraCharacter::GetCharacterLevel_Implementation() const
{
	// Retrieve the Player state
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	// Retrieve the level from the player state
	// For this character, its level is the same as the player level
	return AuraPlayerState->GetPlayerLevel();
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(const int32 Level) const
{
	// Retrieve the Player state
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	return AuraPlayerState->FindAttributePointRewardForLevel(Level);
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(const int32 Level) const
{
	// Retrieve the Player state
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	return AuraPlayerState->FindSpellPointRewardForLevel(Level);
}

int32 AAuraCharacter::FindLevelForXP_Implementation(const int32 XP) const
{
	// Retrieve the Player state
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	return AuraPlayerState->FindLevelForXP(XP);
}

int32 AAuraCharacter::GetPlayerXP_Implementation() const
{
	// Retrieve the Player state
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	return AuraPlayerState->GetPlayerXP();
}

void AAuraCharacter::AddToPlayerXP_Implementation(const int32 XPToAdd)
{
	// Retrieve the Player state
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	AuraPlayerState->AddToPlayerXP(XPToAdd);
}

int32 AAuraCharacter::GetPlayerAttributePoints_Implementation() const
{
	// Retrieve the Player state
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	return AuraPlayerState->GetAttributePoints();
}

void AAuraCharacter::AddToPlayerAttributePoints_Implementation(const int32 PointsToAdd) const
{
	// Retrieve the Player state
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	return AuraPlayerState->AddToAttributePoints(PointsToAdd);
}

int32 AAuraCharacter::GetPlayerSpellPoints_Implementation() const
{
	// Retrieve the Player state
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	return AuraPlayerState->GetSpellPoints();
}

void AAuraCharacter::AddToPlayerSpellPoints_Implementation(const int32 PointsToAdd) const
{
	// Retrieve the Player state
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->AddToSpellPoints(PointsToAdd);
}

void AAuraCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles_Implementation();
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
	
	// Pass references this character of the PlayerState's AuraAbilitySystemComponent and AttributeSet
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	
	// Broadcast the Ability System Component validity
	OnAbilitySystemComponentRegistered.Broadcast(AbilitySystemComponent);
	
	// Bind Stun Tag changed
	AbilitySystemComponent->RegisterGameplayTagEvent(AuraGameplayTagsManager::Debuff_Stun, EGameplayTagEventType::NewOrRemoved).
	AddUObject(this, &AAuraCharacter::StunTagChanged);
	
	// Initialize Attribute values
	InitializeDefaultAttributes();
	
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
	
	// Bindings to Player State
	
	// Level up bindings
	AuraPlayerState->OnPlayerLevelChangedDelegate.AddLambda(
		[&](int32 NewPlayerLevel)
		{
			MulticastLevelUpParticles_Implementation();
		});
	
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		// Initialise and cache the Ability Actor Info on the server as it will have all the information needed.
		InitCharacterAndComponents();
		
		// Adds Startup Abilities to the character
		AddCharacterAbilities();
	}
}

void AAuraCharacter::OnRep_PlayerState()
{
	// Only on clients
	Super::OnRep_PlayerState();
	
	// Initialise and cache the Ability Actor Info on the client as it will have all the information needed.
	InitCharacterAndComponents();
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	// Activate the particle effect
	if (IsValid(LevelUpNiagaraComponent))
	{
		// Rotate the effects towards the camera 
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		
		// Spawn the effect
		LevelUpNiagaraComponent->Activate(true);
	}
}
