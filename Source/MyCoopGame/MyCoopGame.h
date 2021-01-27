// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

extern int32 DebugDrawType;

UENUM()
enum class EDebugDrawType : uint8
{
	DebugDrawType_None=0,
	DebugDrawType_Weapon,
	DebugDrawType_EyeTrace,
};