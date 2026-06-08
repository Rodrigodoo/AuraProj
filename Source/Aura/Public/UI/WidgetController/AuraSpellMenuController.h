

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraSpellMenuController.generated.h"

class UAuraUserWidget;
// Delegates
// Delegate for when an ability is selected
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FButtonSelectedSignature, UAuraUserWidget*, Button);

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
	// Ability Selection Delegate
	UPROPERTY(BlueprintAssignable, Category = "GAS|Spells Menu")
	FButtonSelectedSignature AbilitySelectedDelegate;
	
	// Equip Slot Selection Delegate
	UPROPERTY(BlueprintAssignable, Category = "GAS|Spells Menu")
	FButtonSelectedSignature EquipSlotSelectedDelegate;
	//~ End - Delegates
	
	// Method to broadcast the selection of an Ability
	UFUNCTION(BlueprintCallable, Category = "GAS|Spells Menu")
	void SelectAbility(UAuraUserWidget* AbilityButton) const;
	
	// Method to broadcast the selection of an Equip Slot
	UFUNCTION(BlueprintCallable, Category = "GAS|Spells Menu")
	void SelectEquipSlot(UAuraUserWidget* EquipSlotButton) const;
	
	// Equips the ability for a specific input tag
	UFUNCTION(BlueprintCallable, Category = "GAS|Spells Menu")
	void EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);
	
	// Method to be called whenever a spell point should be spent
	// It will inform the ability system component (server) to update the ability and spend a spell point 
	UFUNCTION(BlueprintCallable, Category = "GAS|Spells Menu")
	void SpendSpellPoint(const FGameplayTag& AbilityTag);
	
	// Retrieves the descriptions for the Ability tag provided
	UFUNCTION(BlueprintCallable, Category = "GAS|Spells Menu")
	void GetDescriptionsForAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

protected:
	// Delegate to broadcast when Spell Points changes
	UPROPERTY(BlueprintAssignable, Category="GAS|SpellPoints")
	FOnStatChangedSignature SpellPointsDelegate;
	
	// Auxiliary method to know if Spend Point Button and Equip Button should be enabled
	UFUNCTION(BlueprintCallable, Category = "GAS|Spells Menu")
	static void ShouldEnableSpendPointAndEquipButtons(const FGameplayTag& StatusTag, bool& bSpendPointEnabled, bool& bEquipEnabled);
};
