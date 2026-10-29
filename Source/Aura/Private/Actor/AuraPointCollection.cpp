


#include "Actor/AuraPointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#define CREATE_AND_SETUP_PT(Pt_X) \
    Pt_X = CreateDefaultSubobject<USceneComponent>(TEXT(#Pt_X)); \
    Points.Add(Pt_X); \
    Pt_X->SetupAttachment(GetRootComponent());

AAuraPointCollection::AAuraPointCollection()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create Root Component
	PointsRootComponent = CreateDefaultSubobject<USceneComponent>("PointsRootComponent");
	Points.Add(PointsRootComponent);
	SetRootComponent(PointsRootComponent);
	
	// Create the Points
	CREATE_AND_SETUP_PT(Pt_1);
	CREATE_AND_SETUP_PT(Pt_2);
	CREATE_AND_SETUP_PT(Pt_3);
	CREATE_AND_SETUP_PT(Pt_4);
	CREATE_AND_SETUP_PT(Pt_5);
	CREATE_AND_SETUP_PT(Pt_6);
	CREATE_AND_SETUP_PT(Pt_7);
	CREATE_AND_SETUP_PT(Pt_8);
	CREATE_AND_SETUP_PT(Pt_9);
	CREATE_AND_SETUP_PT(Pt_10);
	
}

TArray<FTransform> AAuraPointCollection::GetGroundPoints(int32 NumPoints,
                                                         float YawOverride /*=0.f*/,
                                                         float RadiusForPlayerIgnore /*=1500.f*/)
{
	checkf(Points.Num() >= NumPoints, TEXT("Attempted to access Points out of bounds."));
	
	// All the points on the ground
	TArray<FTransform> PointTransforms;

	// Get all the players close to actor's location to be sure to ignore them on the line trace
	TArray<AActor*> IgnoreActors;
	UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius(this, IgnoreActors, TArray<AActor*>(), RadiusForPlayerIgnore,
	                                                     GetActorLocation());
	
	// Loop through the points and get the closest ones to the ground location
	for (USceneComponent* Pt : Points)
	{
		// Rotate points according to the YawOverride
		if (Pt != PointsRootComponent && YawOverride != 0.f)
		{
			// Make all point turn to the Points Root Component
			FVector ToPoint = Pt->GetComponentLocation() - PointsRootComponent->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Pt->SetWorldLocation(PointsRootComponent->GetComponentLocation() + ToPoint);
		}
		
		// Get Raised and Lowered locations for line trace
		const FVector RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 250.f);
		const FVector LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 250.f);
		
		// Make line trace
		FHitResult HitResult;
		UKismetSystemLibrary::LineTraceSingle(this, RaisedLocation, LoweredLocation, TraceTypeQuery1, false,
		                                      IgnoreActors, EDrawDebugTrace::None, HitResult, true);
		if (HitResult.bBlockingHit)
		{
			// If there was a hit, then add it to the out vector
			FTransform PointTransform = Pt->GetComponentTransform();
			
			// Corrected to the Z of the impact point
			const FVector AdjustedVector = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
			PointTransform.SetLocation(AdjustedVector);
			
			// Correct the Rotation to the normal of the Impact Point
			PointTransform.SetRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactPoint).Quaternion());
			
			// Add the Point Transform
			PointTransforms.Add(PointTransform);
		}
		
		// If we have checked all the points then exit
		if (PointTransforms.Num() >= NumPoints)
		{
			break;
		}
	}
	
	return PointTransforms;
}

// Called when the game starts or when spawned
void AAuraPointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}

