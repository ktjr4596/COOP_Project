// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType, Blueprintable)
enum class EPlayerActionName : uint8
{
	ActionName_Crouch = 0,
	ActionName_Jump,
	ActionName_Zoom,
	ActionName_Fire,
	ActionName_Interact,
	ActionName_ReloadAmmo,
	ActionName_OpenInventory,
};

namespace PlayerActionNames
{
	using ActionNameMap = TMap<EPlayerActionName, FName>;

	const ActionNameMap& GetActionNameMap();

	const FName& GetActionName(EPlayerActionName TargetAction);
}