// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WaveManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveEnd);




class AMonsterBaseClass;
class ASpawnTriggerBox;
class ATargetPoint;
class UEnvQuery;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYCOOPGAME_API UWaveManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWaveManager();


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void StartWave(ASpawnTriggerBox* TargetTrigger);

	void Start(const TArray<UClass*>& SpawningActors, const TArray<AActor*>& TargetPoints, int32 SpawnCount,float SpawnRate,bool bIsLoop);

	void EndWave();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnMonster(const TArray<UClass*>& SpawningActors, const TArray<AActor*>& TargetPoints,int32 SpawnCount);

	void HandleQueryFinished(TSharedPtr<struct FEnvQueryResult> Result);

protected:
	bool CheckWaveState();

protected:
	FTimerHandle TimerHandle_ForSpawn;

	FOnWaveStart OnStartWave;

	FOnWaveStart OnEndWave;

	TArray< AMonsterBaseClass*> CurrentSpawnedMonsters;
		
	ASpawnTriggerBox* CurrentSpawnedTrigger;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wave")
	int32 GoalWave;

	int32 CurrentWave;

	bool bIsWaveStart;
};
