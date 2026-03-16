


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
	
	// Get the player controller then get the hit result from under the cursor
	if (const APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get())
	{
		// Get hit result under the cursor
		FHitResult CursorHit;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility,false,CursorHit);
		if (!CursorHit.bBlockingHit)
		{
			// No hit found
			return;
		}
			
		// If there was a hit then broadcast its impact point
		ValidData.Broadcast(CursorHit.ImpactPoint);
	}
}
