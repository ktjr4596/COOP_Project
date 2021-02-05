// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"



UENUM(BlueprintType)
enum class EItemType :uint8
{
	ItemType_Cosumable,
	ItemType_Equipable,
	ItemType_Ammo,
};


UENUM()
enum class EAmmoType
{
	AmmoType_None,
	AmmoType_Rife,
};
