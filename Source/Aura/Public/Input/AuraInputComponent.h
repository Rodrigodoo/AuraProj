

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfigDataAsset.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"

/**
 * Aura project Input Component that should manage the inputs received from an AuraInputConfig Data Asset
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	// Binds input actions for Abilities
	// This will be read from an UAuraInputConfigDataAsset
	// It also takes 3 functions for pressed, released and held actions
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfigDataAsset* InputConfigDataAsset, 
		UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
	
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputConfigDataAsset* InputConfigDataAsset, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	// Early checks
	check(InputConfigDataAsset);

	// Bind functions to all input actions
	// Later GameplayTags will decide what functions will be used
	for (const FAuraInputAction& Action : InputConfigDataAsset->AbilityInputActions)
	{
		// Early check
		if (!Action.InputAction || !Action.InputTag.IsValid())
		{
			continue;
		}
		
		// If there is a valid Pressed function bind it to the Action
		if (PressedFunc)
		{
			// Pass the action's Gameplay Tag onto the Pressed Function
			// ETriggerEvent::Started - called once when an input is pressed/started 
			BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
		}
		
		// If there is a valid Released function bind it to the Action
		if (ReleasedFunc)
		{
			// Pass the action's Gameplay Tag onto the Released Function
			// ETriggerEvent::Completed - called once when an input is released/finished 
			BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
		}
		
		// If there is a valid Held function bind it to the Action
		if (HeldFunc)
		{
			// Pass the action's Gameplay Tag onto the Held Function
			// ETriggerEvent::Triggered - called every frame while the input is held
			BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
		}
	}
	
}
