


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
	OnPlayerXPChangedDelegate.Broadcast(PlayerXP);
}

void AAuraPlayerState::AddToPlayerXP(const int32 XPToAdd)
{
	PlayerXP += XPToAdd;
	OnPlayerXPChangedDelegate.Broadcast(PlayerXP);

	// Check if a level up is required
	// Possible new player level
	const int32 NewPlayerLevel = FindLevelForXP(PlayerXP);
	const int32 NumLevelUps = NewPlayerLevel - PlayerLevel;
	if (NumLevelUps <= 0)
	{
		// There is no need for a level up
		return;
	}
	
	// We loop from the next player level, to the player level + number of LevelUps
	// The methods FindAttributePointRewardForLevel and FindSpellPointRewardForLevel fix the indexes
	for (int Level = PlayerLevel + 1; Level <=  PlayerLevel + NumLevelUps; ++Level)
	{
		// Add the attribute point reward
		const int32 AttributePointReward = FindAttributePointRewardForLevel(Level);
		AddToAttributePoints(AttributePointReward);
		// Add the spell point reward
		const int32 SpellPointReward = FindSpellPointRewardForLevel(Level);
		AddToSpellPoints(SpellPointReward);

		// Add a player level
		AddToPlayerLevel(1);
	}
	
	// If a level up occured maximize Health and Mana (only needs to happen once!)
	if (UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(GetAttributeSet()))
	{
		AuraAttributeSet->MaximizeVitalAttributes();
	}
}

int32 AAuraPlayerState::FindLevelForXP(const int32 XP) const
{
	check(LevelUpInfoDataAsset)
	return LevelUpInfoDataAsset->FindLevelForXP(XP);
}

void AAuraPlayerState::SetAttributePoints(const int32 NewAttributePoints)
{
	AttributePoints = NewAttributePoints;
}

void AAuraPlayerState::AddToAttributePoints(const int32 AttributePointsToAdd)
{
	AttributePoints += AttributePointsToAdd;
}

void AAuraPlayerState::AddToSpellPoints(const int32 SpellPointsToAdd)
{
	SpellPoints += SpellPointsToAdd;
}

int32 AAuraPlayerState::FindAttributePointRewardForLevel(const int32 Level) const
{
	// Since LevelUpInfoData is 0 index based, then we need to remove 1 to the Level get the right position
	check(LevelUpInfoDataAsset)
	check(LevelUpInfoDataAsset->LevelUpInfoData.IsValidIndex(Level - 1))
	return LevelUpInfoDataAsset->LevelUpInfoData[Level - 1].AttributePointReward;
}

int32 AAuraPlayerState::FindSpellPointRewardForLevel(const int32 Level) const
{
	// Since LevelUpInfoData is 0 index based, then we need to remove 1 to the Level get the right position
	check(LevelUpInfoDataAsset)
	check(LevelUpInfoDataAsset->LevelUpInfoData.IsValidIndex(Level - 1))
	return LevelUpInfoDataAsset->LevelUpInfoData[Level - 1].SpellPointReward;
}

void AAuraPlayerState::SetSpellPoints(const int32 NewSpellPoints)
{
	SpellPoints = NewSpellPoints;
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
}
