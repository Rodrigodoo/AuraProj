


#include "AbilitySystem/Abilities/AuraBeamSpell.h"

void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (!HitResult.bBlockingHit)
	{
		EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,false);
		return;
	}
	
	// Cache Hit Result information
	MouseHitLocation = HitResult.ImpactPoint;
	MouseHitActor = HitResult.GetActor();
}

void UAuraBeamSpell::StoreOwnerPlayerController()
{
	// Cache player controller
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
	}
}
