


#include "AuraGameplayTagsManager.h"
#include "GameplayTagsManager.h"

FAuraGameplayTagsManager FAuraGameplayTagsManager::Instance;

void FAuraGameplayTagsManager::InitializeNativeGameplayTags()
{
	Instance.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), 
		FString("Reduces damage taken, improves Block Chance"));
}