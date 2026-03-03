

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraOverlayWidgetController.generated.h"

struct FOnAttributeChangeData;

// Delegates that can be used in BP and called on multiple widgets
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

// Data table row struct design to display messages to screen
// Note: this will add a widget to the screen
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()
	
	// Associated gameplay tag
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();
	
	// Message to display
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText MessageText = FText();
	
	// Widget class to display the message
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UAuraUserWidget> MessageWidget;
	
	// [Optional] Image to display
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
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
	
	// Will later be moved to a static function library
	// Retrieve a data table row by tag
	// Note: The row name must have the same name as the tag. (This is something easy to break but we are using it for educational purposes)
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);	
};

template <typename T>
T* UAuraOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}
