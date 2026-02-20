

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraOverlayWidgetController.generated.h"

struct FOnAttributeChangeData;

// Delegates that can be used in BP and called on multiple widgets
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

/**
 * Overlay Controller responsible for controlling AuraOverlayWidgets
 * BlueprintType - Can be used as type in BP Event Graphs
 * Blueprintable - Can make a blueprint based on this class
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAuraOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	//~ Begin UAuraWidgetController overrides
	// Broadcast initial values
	virtual void BroadcastInitialValues() override;
	
	// Binds callbacks to dependencies
	virtual void BindCallbacksToDependencies() override;
	//~ End UAuraWidgetController overrides
	
	//~ Begin Delegate Variables
	// Health Change Delegate
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;
	
	// Max Health Change Delegate
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;
	
	// Mana Change Delegate
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnManaChangedSignature OnManaChanged;
	
	// Max Mana Change Delegate
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxManaChangedSignature OnMaxManaChanged;
	//~ End Delegate Variables
	
protected:
	//~ Begin Callback methods
	// Called when Health changed
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	// Called when Max Health changed
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	
	// Called when Mana changed
	void ManaChanged(const FOnAttributeChangeData& Data) const;
	// Called when Max Mana changed
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;
	//~ End Callback methods
};
