#include "PlayerActionNames.h"

namespace PlayerActionNames
{

	const ActionNameMap& GetActionNameMap()
	{
		static TMap<EPlayerActionName, FName> PlayerActionMap = {
			{EPlayerActionName::ActionName_Crouch,"Crouch"} ,
			{EPlayerActionName::ActionName_Jump,"Jump"},
			{EPlayerActionName::ActionName_Zoom,"Zoom"},
			{EPlayerActionName::ActionName_Fire,"Fire"},
			{EPlayerActionName::ActionName_Interact,"Interact"},
			{EPlayerActionName::ActionName_ReloadAmmo,"Reload"},
			{EPlayerActionName::ActionName_OpenInventory,"OpenInventory"}
		};

		return PlayerActionMap;
	}

	const FName& GetActionName(EPlayerActionName TargetAction)
	{
		const ActionNameMap& ActionNames= GetActionNameMap();

		return ActionNames[TargetAction];
	}
}