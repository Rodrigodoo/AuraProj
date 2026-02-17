


#include "Character/AuraEnemy.h"

#include "Aura/Aura.h"


AAuraEnemy::AAuraEnemy()
{
	// Set collision to block visibility
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
}

void AAuraEnemy::HighlightActor()
{
	// Do highlight
	// The material used needs a stencil value of 250 for highlight
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	// If this actor has a weapon, then Highlight it
	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(true);
		Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
}

void AAuraEnemy::UnHighlightActor()
{
	// Remove highlight
	// The material used needs a stencil value of 250 for highlight
	GetMesh()->SetRenderCustomDepth(false);

	// If this actor has a weapon, then Unhighlight it
	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(false);
	}
}
