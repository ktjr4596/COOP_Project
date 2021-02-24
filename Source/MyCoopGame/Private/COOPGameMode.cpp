// Fill out your copyright notice in the Description page of Project Settings.


#include "COOPGameMode.h"
#include "Components/HealthComponent.h"
#include "COOPGameState.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Trigger/SpawnTriggerBox.h"
#include "AI/MonsterBaseClass.h"
#include "WaveManager.h"

ACOOPGameMode::ACOOPGameMode()
{
	GameStateClass = ACOOPGameState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;

	WaveManager = CreateDefaultSubobject<UWaveManager>(TEXT("WaveManager"));
}

void ACOOPGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void ACOOPGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (nullptr!= GameState && (0 < GameState->PlayerArray.Num()))
	{
		if (false == CheckAnyPlayerAlive())
		{
			ACOOPGameState* MyState = Cast<ACOOPGameState>(GameState);
			if (true == ensure(MyState))
			{
				MyState->SetGameState(EGameState::GameState_GameOver);
			}
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


void ACOOPGameMode::StartWave(const TArray<UClass*>& SpawningActors, const TArray<AActor*>& TargetPoints, int32 SpawnCount, float SpawnRate, bool bIsLoop)
{
	if (nullptr != WaveManager)
	{
		WaveManager->Start(SpawningActors,TargetPoints,SpawnCount,SpawnRate,bIsLoop);
	}
}
