

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraSpellMenuController.generated.h"

class UAuraUserWidget;
// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilitySelectedSignature, UAuraUserWidget*, AbilityButton);

/**
 * Spell Menu Controller responsible for controlling the Aura Spell Menu Widget
 * BlueprintType - Can be used as type in BP Event Graphs
 * Blueprintable - Can make a blueprint based on this class
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAuraSpellMenuController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	//~ Begin - UAuraWidgetController Overrides
	// Call delegates to broadcast initial values
	virtual void BroadcastInitialValues() override;
	
	// Binds callbacks to dependencies of this controller
	virtual void BindCallbacksToDependencies() override;
	//~ End - UAuraWidgetController Overrides
	
	//~ Begin - Delegates
	UPROPERTY(BlueprintAssignable, Category = "GAS|Spells Menu")
	FAbilitySelectedSignature AbilitySelectedDelegate;
	//~ End - Delegates
	
	// Method to broadcast the selection of an Ability
	UFUNCTION(BlueprintCallable, Category = "GAS|Spells Menu")
	void SelectAbility(UAuraUserWidget* AbilityButton);
	
protected:
	// Delegate to broadcast when Spell Points changes
	UPROPERTY(BlueprintAssignable, Category="GAS|SpellPoints")
	FOnStatChangedSignature SpellPointsDelegate;
};
