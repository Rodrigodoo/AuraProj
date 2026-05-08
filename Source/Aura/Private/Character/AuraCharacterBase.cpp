


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AAuraCharacterBase::AAuraCharacterBase()
{
 	// No tick for this class
	PrimaryActorTick.bCanEverTick = false;
	
	//Make capsule ignore overlap events (this could double event generation with mesh)
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	// Make capsule and mesh ignore camera
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	// Make projectile channel overlap and generate overlap event
	GetMesh()->SetCollisionResponseToChannel(ECC_PROJECTILE, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	// Setup Weapon
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// Fix for generating impact on Client Only
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::InitCharacterAndComponents()
{
	// Nothing to do here
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

EAuraCharacterClass AAuraCharacterBase::GetCharacterClass_Implementation() const
{
	return CharacterClass;
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) const
{
	// Find the correct socket name for the montage being played
	for (const FTaggedMontage& TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag.MatchesTagExact(MontageTag))
		{
			// If found, returns the socket location (world coordinates) of the combat point for the montage being played
			// If all failed returns the Mesh Component Transform (following GetSocketLocation)
			return FindSocketLocation(TaggedMontage.SocketName);
		}
	}
	
	// If all failed, returns the Mesh Component Transform (following GetSocketLocation logic)
	return GetMesh()->GetComponentTransform().GetTranslation();
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die()
{
	// Drop weapon (this is replicated)
	if (IsValid(Weapon))
	{
		Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));
	}

	// Call on all clients
	MulticastHandleDeath();
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bIsDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation() const
{
	return AttackMontages;
}

bool AAuraCharacterBase::FindRandomAttackMontage_Implementation(FTaggedMontage& RandomMontage) const
{
	if (AttackMontages.IsEmpty())
	{
		return false;
	}
	
	// Get a random TaggedMontage struct from the array
	RandomMontage = AttackMontages[FMath::RandRange(0, AttackMontages.Num() - 1)];
	return true;
}

bool AAuraCharacterBase::FindTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag,
                                                               FTaggedMontage& OutTaggedMontage) const
{
	// Find the correct TaggedMontage by montage tag
	for (const FTaggedMontage& TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag.MatchesTagExact(MontageTag))
		{
			// If a valid TaggedMontage was found then return it
			OutTaggedMontage = TaggedMontage;
			return true;
		}
	}
	
	// If none was found return an empty struct
	OutTaggedMontage = FTaggedMontage();
	return false;
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

int32 AAuraCharacterBase::GetMinionCount_Implementation() const
{
	return MinionCount;
}

void AAuraCharacterBase::SetMinionCount_Implementation(const int32 NewMinionCount)
{
	MinionCount = NewMinionCount;
}

void AAuraCharacterBase::IncrementMinionCount_Implementation(int32 Amount)
{
	Amount = FMath::Max(0.f, Amount);
	MinionCount += Amount;
}

void AAuraCharacterBase::DecrementMinionCount_Implementation(int32 Amount)
{
	Amount = FMath::Max(0.f, Amount);
	MinionCount -= Amount;
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	// Play death sound
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	
	// Apply ragdoll for character and weapon
	if (IsValid(Weapon))
	{
		Weapon->SetSimulatePhysics(true);
		Weapon->SetEnableGravity(true);
		Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // Make the mesh collide with the world
	
	// Disable capsule collision
	// For some just disabling the capsule collision was making the health bar on enemies just fall on the floor
	// This might warrant some more investigation
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); 
	
	// Begins Dissolve effect
	Dissolve();
	
	// Mark the character as dead
	bIsDead = true;
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	// NOTE: The order of these calls is important and should be maintained!
	
	// Initialize the Primary Attributes
	// By applying an instant Gameplay Effect to self
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	
	// Initialize the Secondary Attributes
	// By applying an infinite Gameplay Effect to self
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	
	// Initialize the Vital Attributes
	// By applying an infinite Gameplay Effect to self
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AAuraCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffect, const float Level) const
{
	// Early checks
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffect);
	
	// Apply a Gameplay Effect Spec to self
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this); // Add this as source object since it's applying the effect to self
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	// This should only be run in the server
	// As abilities should only be granted at server level
	if (!HasAuthority())
	{
		return;
	}
	
	// Add the startup Abilities to the character
	UAuraAbilitySystemComponent* AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraAbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
	AuraAbilitySystemComponent->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

void AAuraCharacterBase::Dissolve()
{
	// Mesh
	if (IsValid(DissolveMaterialInstance))
	{
		// Create a dynamic material instance and set it on the mesh
		UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMaterialInstance);
	
		// Start the timeline
		StartDissolveTimeline(DynamicMaterialInstance);
	}
	// Weapon
	if (IsValid(Weapon) && IsValid(WeaponDissolveMaterialInstance))
	{
		// Create a dynamic material instance and set it on the weapon
		UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMaterialInstance);
	
		// Start the timeline
		StartWeaponDissolveTimeline(DynamicMaterialInstance);
	}
}

FVector AAuraCharacterBase::FindSocketLocation(const FName SocketName) const
{
	// If there is a mesh, search for the provided socket
	if (GetMesh()->DoesSocketExist(SocketName))
	{
		return GetMesh()->GetSocketLocation(SocketName);
	}
	
	// If the socket was not on the mesh then search in the weapon, if it exists
	if (IsValid(Weapon) && Weapon->DoesSocketExist(SocketName))
	{
		return Weapon->GetSocketLocation(SocketName);
	}
	
	// If all failed, returns the Mesh Component Transform (following GetSocketLocation logic)
	return GetMesh()->GetComponentTransform().GetTranslation();
}
