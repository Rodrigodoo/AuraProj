


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"


AAuraPlayerState::AAuraPlayerState()
{
	// Construct and setup the Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	// Set Replication mode to Mixed so that Gameplay Effects are replicated to Owning Clients
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	// Construct the Attribute Set
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	// Frequency that the server will send updates to clients
	// Note: For games that the ASC is set on player state the frequency needs to be high to make sure replication 
	// occurs at acceptable intervals. In games like Lyra or Fornite it's set around 100.0f
	SetNetUpdateFrequency(100.f);
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Registers the variables to be replicated
	DOREPLIFETIME(AAuraPlayerState, PlayerLevel);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::OnRep_PlayerLevel(int32 OldPlayerLevel)
{
	
}
