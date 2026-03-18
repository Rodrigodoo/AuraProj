


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"


AAuraEnemy::AAuraEnemy()
{
	// Set collision to block visibility
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	
	// Construct and setup the Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	// Set Replication mode to Minimal so that Gameplay Effects are NOT replicated to Clients
	// The AI-Controlled pawns on the client do not need to process these effects as they will be done on the server
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// Construct the Attribute Set
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize ASC
	InitCharacterAndComponents();
}

void AAuraEnemy::HighlightActor()
{
	// Do highlight
	// The material used needs a stencil value of 250 for highlight
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	// If this actor has a weapon, then Highlight it
	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(true);
		Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
}

void AAuraEnemy::UnHighlightActor()
{
	// Remove highlight
	// The material used needs a stencil value of 250 for highlight
	GetMesh()->SetRenderCustomDepth(false);

	// If this actor has a weapon, then Unhighlight it
	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(false);
	}
}

int32 AAuraEnemy::GetCharacterLevel()
{
	return CharacterLevel;
}

void AAuraEnemy::InitCharacterAndComponents()
{
	// Initialises the Ability Actor Info
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		// Inform the ASC that its AbilityActorInfo was set
		// This will bind delegates
		AuraAbilitySystemComponent->AbilityActorInfoSet();
	}
	
	// Initialize Default attributes in the character
	InitializeDefaultAttributes();
}
