

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/AuraEnemyInterface.h"
#include "UI/WidgetController/AuraOverlayController.h"
#include "AuraEnemy.generated.h"


class AAuraAIController;
class UBehaviorTree;
enum class EAuraCharacterClass : uint8;
class UWidgetComponent;
/**
 * Enemy Base Class. All enemy classes should inherit from this
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IAuraEnemyInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();

	//~ Begin - ACharacter Overrides
	// Called whenever this character gets possessed by a controller
	virtual void PossessedBy(AController* NewController) override;
	//~ End - ACharacter Overrides

	//~ Begin - IAuraEnemyInterface
	// Highlights the enemy
	virtual void HighlightActor() override;
	
	// Unhighlights the enemy
	virtual void UnHighlightActor() override;
	
	// Set this enemy's combat target
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	
	// Get this enemy's combat target
	virtual AActor* GetCombatTarget_Implementation() override;
	//~ End - IAuraEnemyInterface
	
	//~ Begin - IAuraCombatInterface
	// Get this enemy's level
	virtual int32 GetCharacterLevel_Implementation() const override;
	
	// Process character's death (Only on server)
	// Drop Weapon and Ragdoll
	virtual void Die(const FVector& DeathImpulse = FVector::ZeroVector) override;
	//~ End - IAuraCombatInterface
	
	//~ Begin -AAuraCharacterBase overrides
	
	// Initializes the character and other components
	// Init the AbilitySystemComponent and stores internal references to AbilitySystemComponent and AttributeSet
	virtual void InitCharacterAndComponents() override;
	//~ End -AAuraCharacterBase overrides
	
	// Delegate to receive health changes
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	
	// Delegate to receive max health changes
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	// Flag to signal if this character is hit reacting
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;
	
	// Base walking speed for the character.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;
	
	// Base walking speed for the character.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;
	
	// Target actor this character if fighting.
	// Used to track where this character should be facing (Facing CombatTarget actor)
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;
	
protected:
	virtual void BeginPlay() override;
	
	//~ Begin - AAuraCharacterBase Overrides
	virtual void InitializeDefaultAttributes() const override;
	//~ End - AAuraCharacterBase Overrides
	
	// Delegate to be called when a Hit React Gameplay Tag is added or removed
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	// The Enemy's Level
	// This value works as an identifier of the enemy's difficulty
	// It will influence its attribute calculations
	// Note: Note replicated because only the server needs to know this information
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Character Class Defaults")
	int32  CharacterLevel = 1;
	
	// Health Bar widget that hover over the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Health Bar")
	TObjectPtr<UWidgetComponent> HealthBarWidgetComponent;
	
	// Behaviour Tree for this actor
	// Defines actions and behaviors this actor can have
	UPROPERTY(EditAnywhere, Category= "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	// Controller for this actor
	// Responsible fo executing the actions this actor can make
	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
};
