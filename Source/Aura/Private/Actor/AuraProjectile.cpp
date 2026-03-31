


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

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
	// This will allow it to be stopped later
	check(FlyingSound) // If it's not valid the audio component will be null
	FlyingAudioComponent = UGameplayStatics::SpawnSoundAttached(FlyingSound,GetRootComponent());
}

void AAuraProjectile::Destroyed()
{
	// If the actor did not overlap an actor, and it is a client
	// Then stop the flight sound and play impact sound and spawn particle system
	if (!bHit && !HasAuthority())
	{
		if (IsValid(FlyingAudioComponent))
		{
			FlyingAudioComponent->Stop();
		}
		
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
	}

	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Stop the flying sound
	FlyingAudioComponent->Stop();
	
	// Play impact sound and spawn particle system
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());

	// If we are the server
	if (HasAuthority())
	{
		// If the other actor as an ASC then apply the Gameplay Effect to it
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
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

