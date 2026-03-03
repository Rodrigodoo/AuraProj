

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraOverlayWidgetController.generated.h"

struct FOnAttributeChangeData;

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

// Delegates that can be used in BP and called on multiple widgets
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);

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
	FOnAttributeChangedSignature OnHealthChanged;
	
	// Max Health Change Delegate
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	// Mana Change Delegate
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;
	
	// Max Mana Change Delegate
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;
	
	// Message Widget Row Delegate
	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;
	//~ End Delegate Variables
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
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
