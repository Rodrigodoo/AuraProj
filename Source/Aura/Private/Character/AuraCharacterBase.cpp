


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"

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
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

FVector AAuraCharacterBase::GetCombatSocketLocation() const
{
	check(Weapon);
	
	// Returns the socket location (world coordinates) of the weapon's tip
	// If not found will return the component transform (world coordinates)
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
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
}
