// Fill out your copyright notice in the Description page of Project Settings.


#include "COOPGameMode.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


void ACOOPGameMode::BeginPlay()
{
	Super::BeginPlay();



}

//void ACOOPGameMode::HandleQueryFinished(UEnvQueryInstanceBlueprintWrapper * QueryInstance, EEnvQueryStatus::Type QueryStatus)
//{
//
//}

void ACOOPGameMode::RunSpawnQuery(class UClass* SpawningActor)
{
	if (nullptr != SpawningActor)
	{
		SpawningActors.Add(SpawningActor);
	}
	FEnvQueryRequest QueryRequest = FEnvQueryRequest(TargetEnvQuery, this);
	QueryRequest.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &ACOOPGameMode::HandleQueryFinished);
}

void ACOOPGameMode::HandleQueryFinished(TSharedPtr<struct FEnvQueryResult> Result)
{
	if (true == Result.Get()->IsSuccsessful())
	{
		TArray<FVector> OutLocations;
		Result.Get()->GetAllAsLocations(OutLocations);
	
		UWorld* CurrentWorld= GetWorld();
		if (nullptr == CurrentWorld)
		{
			return;
		}
		if (1 < OutLocations.Num())
		{
			CurrentWorld->SpawnActor(SpawningActors[0], &OutLocations[0], &FRotator::ZeroRotator);
		}
	}
}
