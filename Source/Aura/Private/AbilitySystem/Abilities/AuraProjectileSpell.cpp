


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTagsManager.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/AuraCombatInterface.h"

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	// If we are not on the server then move along.
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}
	
	// If the Avatar implements a combat interface, 
	// then get the weapon's socket location to spawn the projectile
	IAuraCombatInterface* CombatInterface = Cast<IAuraCombatInterface>(GetAvatarActorFromActorInfo());
	if (!CombatInterface)
	{
		// Otherwise, for now we will not spawn the projectile
		return;
	}
	
	// Get the socket location from the interface
	const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
	
	// Get the rotation of the projectile to point at the target location
	// Vector from socket location to target location
	FRotator SpawnRotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	// Make the projectile fly parallel to the ground
	SpawnRotation.Pitch = 0.0f;
	
	// Spawn the projectile actor at the socket location and with the direction of the target
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());
	
	// Begin Spawning
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(), 
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	// Setup Projectile
	if (const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()))
	{
		// Create Custom Effect Context Handle
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(Projectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		FHitResult HitResult;
		HitResult.Location = ProjectileTargetLocation;
		EffectContextHandle.AddHitResult(HitResult);
		
		// Create a Spec Handle to pass it to the Projectile
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
		
		// Get the Gameplay Tags manager to set a Caller by Magnitude to this Spec
		// Key will be Damage tag && Value will be the Ability's damage at its current level
		const FAuraGameplayTagsManager GameplayTagsManager = FAuraGameplayTagsManager::Get();
		const float ScaledDamage = Damage.GetValueAtLevel(10/*GetAbilityLevel()*/);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
			SpecHandle, GameplayTagsManager.Damage, ScaledDamage);
		
		// Pass the Spec Handle down to the Projectile
		Projectile->DamageEffectSpecHandle = SpecHandle;
	}

	// Finish spawning
	Projectile->FinishSpawning(SpawnTransform);
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
