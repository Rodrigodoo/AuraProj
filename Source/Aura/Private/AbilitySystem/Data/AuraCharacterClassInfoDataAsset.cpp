


#include "AbilitySystem/Data/AuraCharacterClassInfoDataAsset.h"

FAuraCharacterClassDefaultInfo UAuraCharacterClassInfoDataAsset::GetClassDefaultInfo(
	const EAuraCharacterClass CharacterClass) const
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}
