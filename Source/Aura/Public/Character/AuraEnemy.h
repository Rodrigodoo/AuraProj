

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AuraCharacterClassInfoDataAsset.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/AuraEnemyInterface.h"
#include "UI/WidgetController/AuraOverlayController.h"
#include "AuraEnemy.generated.h"


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

	//~ Begin - IAuraEnemyInterface
	// Highlights the enemy
	virtual void HighlightActor() override;
	
	// Unhighlights the enemy
	virtual void UnHighlightActor() override;
	//~ End - IAuraEnemyInterface
	
	//~ Begin - IAuraCombatInterface
	// Get this enemy's level
	virtual int32 GetCharacterLevel() override;
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
protected:
	virtual void BeginPlay() override;
	
	//~ Begin - AAuraCharacterBase Overrides
	virtual void InitializeDefaultAttributes() const override;
	//~ End - AAuraCharacterBase Overrides
	
	// The Enemy's Level
	// This value works as an identifier of the enemy's difficulty
	// It will influence its attribute calculations
	// Note: Note replicated because only the server needs to know this information
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Character Class Defaults")
	int32  CharacterLevel = 1;
	
	// This Character's RPG Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Character Class Defaults")
	EAuraCharacterClass CharacterClass = EAuraCharacterClass::Warrior;
	
	// Health Bar widget that hover over the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Health Bar")
	TObjectPtr<UWidgetComponent> HealthBarWidgetComponent;
};
