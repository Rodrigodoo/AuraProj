


#include "Character/AuraEnemy.h"

#include "AuraGameplayTagsManager.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"


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
	
	// Create the Health Bar Widget Component
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthBarWidgetComponent");
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize character
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	
	// Initialize ASC
	InitCharacterAndComponents();
	
	// Grant Initial Abilities
	UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);

	// Set the Health Bar Widget controller
	// The Enemy actor will actually be the controller
	if (UAuraUserWidget* HealthBarWidget = Cast<UAuraUserWidget>(HealthBarWidgetComponent->GetUserWidgetObject()))
	{
		HealthBarWidget->SetWidgetController(this);
	}
	
	// Bind delegates to health and max health changes
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	}	
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	}	
	);
	
	// Bind to Effect.HitReact Gameplay Tag changes\
	// Using EGameplayTagEventType::NewOrRemoved
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTagsManager::Get().Effects_HitReact).AddUObject(this, &AAuraEnemy::HitReactTagChanged);
	
	// Broadcast initial values
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	// Initializes the character information based on Level and RPG Class
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(
		this, CharacterClass, CharacterLevel,AbilitySystemComponent);
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	// Signal if character is hit reacting
	bHitReacting = NewCount > 0;
	if (!bHitReacting)
	{
		// Leave if there are no tags	
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		return;
	}
	
	// Stun the character
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
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
