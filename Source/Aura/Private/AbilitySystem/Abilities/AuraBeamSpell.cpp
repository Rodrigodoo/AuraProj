


#include "AbilitySystem/Abilities/AuraBeamSpell.h"

#include "AuraGameplayTagsManager.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/AuraCombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

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

void UAuraBeamSpell::StoreOwnerVariables()
{
	// Cache player controller
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (!OwnerCharacter->Implements<UAuraCombatInterface>())
	{
		// Owner needs to implement the combat interface
		return;
	}

	// Get the weapon's socket location as the Beam Start Location
	FVector BeamStartLocation = IAuraCombatInterface::Execute_GetCombatSocketLocation(OwnerCharacter,AuraGameplayTagsManager::Montage_Attack_Weapon);
	
	// Do a sphere trace from the Beam Start to the beam Target and Ignore the owner character
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);
	UKismetSystemLibrary::SphereTraceSingle(
		OwnerCharacter, 
		BeamStartLocation, 
		BeamTargetLocation, 
		10.f,
		TraceTypeQuery1, 
		false, 
		ActorsToIgnore, 
		EDrawDebugTrace::None,
		HitResult, 
		true);

	// If there was a valid target between the beam start and end points then update that the mouse hit information
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
}

void UAuraBeamSpell::StoreAdditionalTarget(TArray<AActor*>& OutAdditionalTargets) const
{
	// Find all actors within a certain radius of the hit location that are not the caster and other players
	TArray<AActor*> OverlappingActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);
	ActorsToIgnore.Add(MouseHitActor);
	UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius(OwnerCharacter, OverlappingActors, ActorsToIgnore, 850.f,
	                                                     MouseHitLocation, ACTOR_PlAYER_TAG);
	
	// From those actors choose the closest ones (up to the available amount of beams per ability level)
	const int32 NumAdditionalTargets = AdditionalBeams.GetValueAtLevel(GetAbilityLevel());
	UAuraAbilitySystemLibrary::GetClosestActors(
		NumAdditionalTargets, 
		OverlappingActors, 
		OutAdditionalTargets, 
		MouseHitLocation);
}
