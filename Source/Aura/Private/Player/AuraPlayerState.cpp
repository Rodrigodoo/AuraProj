


#include "Player/AuraPlayerState.h"

AAuraPlayerState::AAuraPlayerState()
{
	// Frequency that the server will send updates to clients
	// Note: For games that the ASC is set on player state the frequency needs to be high to make sure replication 
	// occurs at acceptable intervals. In games like Lyra or Fornite it's set around 100.0f
	NetUpdateFrequency = 100.f;
}
