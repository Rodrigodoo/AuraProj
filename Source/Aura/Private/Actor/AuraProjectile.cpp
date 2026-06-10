


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

AAuraProjectile::AAuraProjectile()
{
 	// Actor won't tick
	PrimaryActorTick.bCanEverTick = false;
	
	// This class must be replicated as it will spawn on the server 
	// only then should it be passed down to the clients
	bReplicates = true;

	// Create Sphere Component and set it as the root
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	
	// Setup Sphere setting (collision only with world or pawn)
	Sphere->SetCollisionObjectType(ECC_PROJECTILE);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	// Create Projectile Component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	
	// Projectile Movement setup
	ProjectileMovementComponent->InitialSpeed = 550;
	ProjectileMovementComponent->MaxSpeed = 550;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	// Set actor's life span
	SetLifeSpan(LifeSpan);
	
	// Bind sphere overlap to AAuraProjectile::OnSphereOverlap
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	
	// Spawn the sound attached to the root component and track it in FlyingAudioComponent
	// This sound will stop when the attached component is destroyed
	UGameplayStatics::SpawnSoundAttached(FlyingSound, GetRootComponent(), NAME_None, 
		FVector::Zero(), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
}

void AAuraProjectile::PlayEffectsOnHit()
{
	// Play impact sound and spawn particle system
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
	bHit = true;
}

void AAuraProjectile::Destroyed()
{
	// If the actor did not overlap an actor, and it is a client
	// Then stop the flight sound and play impact sound and spawn particle system
	if (!bHit && !HasAuthority())
	{
		PlayEffectsOnHit();
	}

	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* ProjectileInstigator = GetInstigator();
	if (!IsValid(ProjectileInstigator))
	{
		// If the Instigator is invalid, destroy this projectile (without causing damage or executing the VFX/SFX).
		// NOTE: This handles the case when Destroy() has already being called on the Instigator (we currently set
		// a brief lifespan on death for enemies), so it's either pending kill or null.
		// FIXME: Find a way to allow for a projectile with a dead Instigator to do damage.
		// Currently, the Instigator is being used for friendly fire validation.
		bHit = true;
		Destroy();
		return;
	}
	// If the causer of the effect is invalid, is trying to hit itself, or it's trying to do friendly fire, then ignore!
	if (!DamageEffectParams.SourceAbilitySystemComponent 
		|| OtherActor == DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor() 
		|| !UAuraAbilitySystemLibrary::IsNotFriend(OtherActor, ProjectileInstigator))
	{
		return;
	}

	// If no hit has been registered, spawn sound and effect at impact point
	if (!bHit)
	{
		// Play impact sound and spawn particle system
		PlayEffectsOnHit();
	}

	// If we are the server
	if (HasAuthority())
	{
		// If the other actor as an ASC then apply the Gameplay Effect to it
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageGameplayEffect(DamageEffectParams);
		}
		
		// Destroy the projectile
		Destroy();
	}
	else
	{
		// Signal in the client that a target was hit
		bHit = true;
	}
}

