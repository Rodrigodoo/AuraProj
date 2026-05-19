

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AuraWidgetController.generated.h"


class UAuraAbilityInfoDataAsset;
class UAuraAttributeSet;
class UAuraAbilitySystemComponent;
class AAuraPlayerState;
class AAuraPlayerController;
class UAbilitySystemComponent;
class UAttributeSet;
struct FAuraAbilityInfo;

// Delegates that can be used in BP and called on multiple widgets
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, AbilityInfo);

/**
 * Struct in charge or recording the model classes to retrieve data from
 */
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	
	FWidgetControllerParams(){};
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		:PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Controller")
	TObjectPtr<APlayerController> PlayerController = nullptr;
	
	// Player State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Controller")
	TObjectPtr<APlayerState> PlayerState = nullptr;
	
	// Ability System Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Controller")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	// Attribute Set
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Controller")
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

/**
 * Controller for all the widgets in the projects assign to it
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	// Setter function for Widget Controller Params
	UFUNCTION(BlueprintCallable, Category = "Widget Controller")
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);
	
	// Call delegates to broadcast initial values
	UFUNCTION(BlueprintCallable, Category = "Widget Controller")
	virtual void BroadcastInitialValues();
	
	// Binds callbacks to dependencies of this controller
	virtual void BindCallbacksToDependencies();
	
	// Broadcasts all active abilities' information
	void BroadcastAbilityInfo();
	
	// Ability Info Delegate
	UPROPERTY(BlueprintAssignable, Category = "GAS|Abilities")
	FAbilityInfoSignature AbilityInfoDelegate;
protected:
	// Data Asset with Ability Information
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAuraAbilityInfoDataAsset> AbilityInfoDataAsset;
	
	//~ Begin Model Section
	// Player Controller
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<APlayerController> PlayerController;
	
	// Player State
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<APlayerState> PlayerState;
	
	// Ability System Component
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	// Attribute Set
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UAttributeSet> AttributeSet;
	
	// Aura Player Controller
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<AAuraPlayerController> AuraPlayerController;
	
	// Aura Player State
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<AAuraPlayerState> AuraPlayerState;
	
	// Aura Ability System Component
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	
	// Aura Attribute Set
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;
	//~ End Model Section
	
	//~ Begin - Model Getters
	// Retrieves the Aura Player Controller
	AAuraPlayerController* GetAuraPlayerController();
	
	// Retrieves the Aura Player State
	AAuraPlayerState* GetAuraPlayerState();
	
	// Retrieves the Aura Ability System Component
	UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent();
	
	// Retrieves the Aura Attribute Set
	UAuraAttributeSet* GetAuraAttributeSet();
	//~ End - Model Getters
	
};
