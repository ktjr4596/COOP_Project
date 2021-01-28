// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MyCoopGame.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, MyCoopGame, "MyCoopGame" );

int32 DebugDrawType = 0;

FAutoConsoleVariableRef CVARDebugType(TEXT("COOP.DebugDrawing"), DebugDrawType, TEXT("Draw Debug Lines for weapons"), ECVF_Cheat);
