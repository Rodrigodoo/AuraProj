


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"


AAuraPlayerState::AAuraPlayerState()
{
	// Construct and setup the Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	// Set Replication mode to Mixed so that Gameplay Effects are replicated to Owning Clients
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	// Construct the Attribute Set
	AttributeSet = CreateDefaultSubobject<UAttributeSet>("AttributeSet");
	
	// Frequency that the server will send updates to clients
	// Note: For games that the ASC is set on player state the frequency needs to be high to make sure replication 
	// occurs at acceptable intervals. In games like Lyra or Fornite it's set around 100.0f
	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
