


#include "Actor/AuraMagicCircle.h"

#include "Components/DecalComponent.h"


AAuraMagicCircle::AAuraMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecal");
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

void AAuraMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

