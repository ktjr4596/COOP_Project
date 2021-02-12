// Fill out your copyright notice in the Description page of Project Settings.


#include "COOPGameMode.h"
#include "Components/HealthComponent.h"
#include "COOPGameState.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Trigger/SpawnTriggerBox.h"


ACOOPGameMode::ACOOPGameMode()
{
	GameStateClass = ACOOPGameState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void ACOOPGameMode::BeginPlay()
{
	Super::BeginPlay();



}

//void ACOOPGameMode::HandleQueryFinished(UEnvQueryInstanceBlueprintWrapper * QueryInstance, EEnvQueryStatus::Type QueryStatus)
//{
//
//}

void ACOOPGameMode::RunSpawnQuery( ASpawnTriggerBox* TargetTrigger, class UClass* SpawningActor)
{
	if (nullptr != SpawningActor)
	{
		SpawningActors.Add(SpawningActor);
	}
	FEnvQueryRequest QueryRequest = FEnvQueryRequest(TargetTrigger->GetEnvQuery(), TargetTrigger);
	QueryRequest.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &ACOOPGameMode::HandleQueryFinished);
}

void ACOOPGameMode::HandleQueryFinished(TSharedPtr<struct FEnvQueryResult> Result)
{
	if (true == Result.Get()->IsSuccsessful())
	{
		TArray<FVector> OutLocations;
		Result.Get()->GetAllAsLocations(OutLocations);

		UWorld* CurrentWorld = GetWorld();
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


void ACOOPGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (false == CheckAnyPlayerAlive())
	{
		ACOOPGameState* MyState = Cast<ACOOPGameState>(GameState);
		if (true==ensure(MyState))
		{
			MyState->SetGameState(EGameState::GameState_GameOver);
		}
	}
}


bool ACOOPGameMode::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		APlayerController* PlayerController = Iter->Get();
		APawn* MyPawn = PlayerController->GetPawn();
		if (nullptr != PlayerController && nullptr != MyPawn)
		{
			UHealthComponent* HealthComp = Cast<UHealthComponent>(MyPawn->GetComponentByClass(UHealthComponent::StaticClass()));
			if (ensure(HealthComp) && 0.0f < HealthComp->GetHealth())
			{
				return true;
			}
		}
	}
	return false;
}
