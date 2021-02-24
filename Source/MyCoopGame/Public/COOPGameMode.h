// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "COOPGameMode.generated.h"

class UWaveManager;
class ASpawnTriggerBox;
class UEnvQuery;



/**
 * 
 */
UCLASS()
class MYCOOPGAME_API ACOOPGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ACOOPGameMode();
public:
	UFUNCTION(BlueprintCallable, Category = "GameState")
	bool CheckAnyPlayerAlive();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void StartWave(const TArray<UClass*>& SpawningActors, const TArray<AActor*>& TargetPoints, int32 SpawnCount, float SpawnRate, bool bIsLoop);

protected:
	virtual void BeginPlay() override;
	
protected:

	virtual void Tick(float DeltaSeconds) override;



protected:
	UWaveManager* WaveManager;

};
