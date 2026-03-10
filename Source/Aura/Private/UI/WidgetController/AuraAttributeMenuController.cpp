


#include "UI/WidgetController/AuraAttributeMenuController.h"

#include "AuraGameplayTagsManager.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraAttributeInfoDataAsset.h"

void UAuraAttributeMenuController::BroadcastInitialValues()
{
	// Get the Attribute Info from the Attribute Info data Asset
	check(AuraAttributeInfoDataAsset)
	FAuraAttributeInfo Info = AuraAttributeInfoDataAsset->FindAttributeInfoFromTag(FAuraGameplayTagsManager::Get().Attributes_Primary_Strength);


	// Read the value of the attribute from the Attribute Set and apply it to the Attribute Info struct to be boardcast
	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	Info.AttributeValue = AuraAttributeSet->GetStrength();
	
	// Broadcast to listeners
	AttributeInfoDelegate.Broadcast(Info);
}

void UAuraAttributeMenuController::BindCallbacksToDependencies()
{
	
}
