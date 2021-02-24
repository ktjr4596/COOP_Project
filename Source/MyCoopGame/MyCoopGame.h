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



#define SURFACETYPE_FLESHDEFAULT			EPhysicalSurface::SurfaceType1
#define SURFACETYPE_FLESHVLUNERABLE			EPhysicalSurface::SurfaceType2

#define COLLISION_WEAPON			ECC_GameTraceChannel1
#define COLLISION_INTERACT			ECC_GameTraceChannel3
#define COLLISION_ITEM				ECC_GameTraceChannel4