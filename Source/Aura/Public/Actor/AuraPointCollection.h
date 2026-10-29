

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraPointCollection.generated.h"

/*
 * Actor that generates a point collection within a radius of a central point
 */
UCLASS()
class AURA_API AAuraPointCollection : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraPointCollection();
	
	// Retrieve the points on the ground for the scene components
	UFUNCTION(BlueprintPure)
	TArray<FTransform> GetGroundPoints(int32 NumPoints, float YawOverride = 0.f, float RadiusForPlayerIgnore = 1500.f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Root component for the points (Equivalent to Pt_0)
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> PointsRootComponent;
	
	// Point collection of scene components
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<TObjectPtr<USceneComponent>> Points;
	
	//~ Start - Points
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_1;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_2;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_3;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_4;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_5;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_6;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_7;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_8;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_9;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_10;
	//~ End - Points
	

};
