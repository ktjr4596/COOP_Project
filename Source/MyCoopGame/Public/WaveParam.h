// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


struct FWaveParams
{


	TArray<class UClass*> SpawningActors;

	TArray<class ATargetPoint*> SpanwedPoints;

	class UEnvQuery * TargetEnvQuery;
};