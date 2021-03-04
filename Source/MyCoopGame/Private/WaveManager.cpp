// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"
#include "Trigger/SpawnTriggerBox.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "AI/MonsterBaseClass.h"
#include "Engine/TargetPoint.h"

// Sets default values for this component's properties
UWaveManager::UWaveManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	GoalWave = 5;
	CurrentWave = 0;
	// ...
}


// Called when the game starts
void UWaveManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (ROLE_Authority==GetOwnerRole())
	{
		
		TArray<AActor*> OutAllActors;
		UGameplayStatics::GetAllActorsOfClass(this, ASpawnTriggerBox::StaticClass(), OutAllActors);

		for (int32 ii = 0; ii < OutAllActors.Num(); ++ii)
		{
			ASpawnTriggerBox* TriggerBox = Cast<ASpawnTriggerBox>(OutAllActors[ii]);
			if (nullptr != TriggerBox)
			{
				
				OnStartWave.AddDynamic(TriggerBox, &ASpawnTriggerBox::HandleStartWave);
				OnEndWave.AddDynamic(TriggerBox, &ASpawnTriggerBox::HandleEndWave);
			}

		}

	}
}

void UWaveManager::SpawnMonster(const TArray<UClass*>& SpawningActors, const TArray<AActor*>& TargetPoints, int32 SpawnCount)
{
	if (false == bIsWaveStart)
	{
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	for (int32 ii = 0; ii < SpawnCount; ++ii)
	{
		int32 PickedMonsterIndex = FMath::RandRange(0, SpawningActors.Num() - 1);
		int32 PickedPointIndex = FMath::RandRange(0, TargetPoints.Num() - 1);

		FVector SpawnedLocation = TargetPoints[PickedPointIndex]->GetActorLocation();

		AMonsterBaseClass* SpawnedMonster = Cast<AMonsterBaseClass>(GetWorld()->SpawnActor(SpawningActors[PickedMonsterIndex], &SpawnedLocation, &FRotator::ZeroRotator, SpawnParams));

		if (nullptr != SpawnedMonster)
		{
			CurrentSpawnedMonsters.Add(SpawnedMonster);
		}
	}
}

void UWaveManager::HandleQueryFinished(TSharedPtr<struct FEnvQueryResult> Result)
{
	if (true == Result->IsSuccsessful())
	{
		TArray<FVector> ResultLocations;
		Result->GetAllAsLocations(ResultLocations);
		if (1 < ResultLocations.Num())
		{
			AMonsterBaseClass* SpawnedMonster= CurrentSpawnedTrigger->SpawnMonster(ResultLocations[0]);
			if (nullptr!=SpawnedMonster)
			{
				CurrentSpawnedMonsters.Add(SpawnedMonster);
			}
		}
		++CurrentWave;
		if (CurrentWave == GoalWave)
		{
			bIsWaveStart = false;
		}

	}

	
}

bool UWaveManager::CheckWaveState()
{
	if (false == bIsWaveStart)
	{
		return false;
	}

	if (CurrentWave != GoalWave)
	{
		return false;
	}

	for (int32 ii = 0; ii < CurrentSpawnedMonsters.Num(); ++ii)
	{
		if (false == CurrentSpawnedMonsters[ii]->IsDied())
		{
			return false;
		}
	}

	return true;
}


// Called every frame
void UWaveManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (true == CheckWaveState())
	{
		EndWave();
	}
	// ...
}

void UWaveManager::StartWave(ASpawnTriggerBox* TargetTrigger)
{
	if (nullptr == TargetTrigger || CurrentSpawnedTrigger==TargetTrigger)
	{
		return;
	}
	bIsWaveStart = true;
	CurrentSpawnedTrigger = TargetTrigger;

	FTimerDelegate TimerDelegateFunc;
	TimerDelegateFunc.BindUFunction(this,TEXT("SpawnMonster"), 2);

	GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle_ForSpawn, TimerDelegateFunc,5.0f,true);
	//OnStartWave.Broadcast(CurrentSpawnedTrigger);

}

void UWaveManager::Start(const TArray<UClass*>& SpawningActors, const TArray<AActor*>& TargetPoints, int32 SpawnCount, float SpawnRate, bool bIsLoop)
{
	if (SpawningActors.Num()<1 ||  TargetPoints.Num()<1)
	{
		return;
	}

	bIsWaveStart = true;
	FTimerDelegate TimerDelegateFunc;
	TimerDelegateFunc.BindUFunction(this, TEXT("SpawnMonster"), SpawningActors, TargetPoints, SpawnCount);

	GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle_ForSpawn, TimerDelegateFunc, SpawnRate, bIsLoop);
	OnStartWave.Broadcast();

}

void UWaveManager::EndWave()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandle_ForSpawn);
	//OnEndWave.Broadcast(CurrentSpawnedTrigger);
	bIsWaveStart = false;
}


