


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraLevelUpInfoDataAsset.h"
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
	DOREPLIFETIME(AAuraPlayerState, PlayerXP);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::SetPlayerLevel(const int32 NewPlayerLevel)
{
	PlayerLevel = NewPlayerLevel;
	OnPlayerLevelChangedDelegate.Broadcast(PlayerLevel);
}

void AAuraPlayerState::AddToPlayerLevel(const int32 LevelsToAdd)
{
	PlayerLevel += LevelsToAdd;
	OnPlayerLevelChangedDelegate.Broadcast(PlayerLevel);
}

void AAuraPlayerState::SetPlayerXP(const int32 NewXP)
{
	PlayerXP = NewXP;
	CurrentLevelPercentage = FindCurrentLevelPercent(PlayerXP);
	OnPlayerXPChangedDelegate.Broadcast(PlayerXP);
	OnPlayerLevelPercentageChangedDelegate.Broadcast(CurrentLevelPercentage);
}

void AAuraPlayerState::AddToPlayerXP(const int32 XPToAdd)
{
	PlayerXP += XPToAdd;
	CurrentLevelPercentage = FindCurrentLevelPercent(PlayerXP);
	OnPlayerXPChangedDelegate.Broadcast(PlayerXP);
	OnPlayerLevelPercentageChangedDelegate.Broadcast(CurrentLevelPercentage);
}

float AAuraPlayerState::FindCurrentLevelPercent(const int32 XP) const
{
	if (LevelUpInfoDataAsset)
	{
		return LevelUpInfoDataAsset->FindCurrentLevelPercent(XP);
	}
	
	return 0.f;
}

void AAuraPlayerState::OnRep_PlayerLevel(int32 OldPlayerLevel)
{
	// Signal that the player Level changed
	OnPlayerLevelChangedDelegate.Broadcast(PlayerLevel);
}

void AAuraPlayerState::OnRep_PlayerXP(int32 OldPlayerXP)
{
	// Signal that the player XP changed
	OnPlayerXPChangedDelegate.Broadcast(PlayerXP);
	CurrentLevelPercentage = FindCurrentLevelPercent(PlayerXP);
	OnPlayerLevelPercentageChangedDelegate.Broadcast(CurrentLevelPercentage);
}
