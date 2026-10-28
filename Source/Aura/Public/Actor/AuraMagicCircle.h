

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraMagicCircle.generated.h"

/*
 * Magic Circle actor to display spell on ground
 */
UCLASS()
class AURA_API AAuraMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraMagicCircle();
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	// Decall to be displayed on the ground
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> MagicCircleDecal;
};
