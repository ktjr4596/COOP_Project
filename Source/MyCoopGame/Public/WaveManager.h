// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WaveManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveStart, class ASpawnTriggerBox*, CurrentTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveEnd,class ASpawnTriggerBox*, CurrentTrigger);




class AMonsterBaseClass;
class ASpawnTriggerBox;

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

	void EndWave();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnMonster(int32 SpawnCount);

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
