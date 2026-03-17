


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
	// Note: using this method instead of ActorInfo->IsLocallyControlled as it adds more checks, and it's not a critical method.
	if (IsLocallyControlled())
	{
		// Client
		SendMouseCursorData();
	}
	else
	{
		//Server
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey PredictionKey = GetActivationPredictionKey();
		
		// Grab the delegate for AbilityTargetDataSet and bind it to OnTargetDataReplicatedCallback
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey).
		AddUObject(this, &UAuraTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		// If the data has already arrived (before activation) ,
		// Call again the delegate to retrieve the data
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey);
		if (!bCalledDelegate)
		{
			// If the data has not yet arrived then the server needs to wait for it
			SetWaitingOnRemotePlayerData();
		}
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

void UAuraTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle,
                                                               FGameplayTag ActivationTag)
{
	// Inform the ASC that the data was received and there is no need to cache it any more
	// Note: this should always be called on the server before consuming the data so ASC no longer keeps a reference to it.
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	
	// Broadcast the data handle locally (on server)
	// Note: should always call ShouldBroadcastAbilityTaskDelegates before broadcasting (read method description)
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(TargetDataHandle);
	}
	
}
