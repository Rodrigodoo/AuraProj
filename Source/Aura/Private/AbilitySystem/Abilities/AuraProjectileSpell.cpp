


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/AuraCombatInterface.h"

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& MontageTag, 
	const bool bOverridePitch /*= false*/, const float PitchOverride /*= 0.f*/)
{
	// If we are not on the server then move along.
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}
	
	// If the Avatar implements a combat interface, 
	// then get the weapon's socket location to spawn the projectile
	if (!GetAvatarActorFromActorInfo()->Implements<UAuraCombatInterface>())
	{
		// Otherwise, for now we will not spawn the projectile
		return;
	}
	
	// Get the socket location from the interface (for the montage playing)
	const FVector SocketLocation = IAuraCombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(), MontageTag);
	
	// Get the rotation of the projectile to point at the target location
	// Vector from socket location to target location
	FRotator SpawnRotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		// If the pitch override flag is set, then override the pitch
		SpawnRotation.Pitch = PitchOverride;
	}
	
	// Spawn the projectile actor at the socket location and with the direction of the target
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());
	
	// Begin Spawning the current amount of projectiles
	for (int Index = 0; Index < ProjectileCount.GetValueAtLevel(GetAbilityLevel()); Index++)
	{
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(), 
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
		// Setup Projectile
		Projectile->DamageEffectParams = MakaDamageEffectParamsFromClassDefaults();

		// Finish spawning
		Projectile->FinishSpawning(SpawnTransform);
	}
}

int32 UAuraProjectileSpell::GetProjectileCount(const int32 Level) const
{
	return ProjectileCount.GetValueAtLevel(Level);
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
