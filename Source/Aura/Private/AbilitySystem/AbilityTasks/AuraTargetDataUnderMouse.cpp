


#include "AbilitySystem/AbilityTasks/AuraTargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UAuraTargetDataUnderMouse* UAuraTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UAuraTargetDataUnderMouse* NewAbilityTaskObj = NewAbilityTask<UAuraTargetDataUnderMouse>(OwningAbility);
	return NewAbilityTaskObj;
}

void UAuraTargetDataUnderMouse::Activate()
{
	Super::Activate();
	
	// Check if Locally controlled 
	// Note: using this methos instead of ActorInfo->IsLocallyControlled as it adds more checks, and it's not a critical method.
	if (IsLocallyControlled())
	{
		// Client
		SendMouseCursorData();
	}
	else
	{
		//Server
		// TODO: We are on the server, so listen for TargetData
	}

	
}

void UAuraTargetDataUnderMouse::SendMouseCursorData()
{
	// Get the player controller then get the hit result from under the cursor
	const APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (!PlayerController)
	{
		// No player controller found we can return
		return;
	}
	
	// Get hit result under the cursor
	FHitResult CursorHit;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility,false,CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		// No hit found, return
		return;
	}
	
	// Create a scoped prediction window
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());
	
	// A hit result was found so pass it to the TargetData
	FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit();
	TargetData->HitResult = CursorHit;
	
	// Send the data to the server
	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(TargetData);
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(), 
		TargetDataHandle, 
		FGameplayTag(), 
		AbilitySystemComponent->ScopedPredictionKey);
	
	// Broadcast the data handle locally (on client)
	// Note: should always call ShouldBroadcastAbilityTaskDelegates before broadcasting (read method description)
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(TargetDataHandle);
	}
	
}
