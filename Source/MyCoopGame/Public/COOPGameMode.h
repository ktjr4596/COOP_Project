// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "COOPGameMode.generated.h"

class UWaveManager;
class ASpawnTriggerBox;
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
	void RunSpawnQuery(class ASpawnTriggerBox* TargetTrigger, class UClass* SpawningActor);

protected:
	virtual void BeginPlay() override;
	
protected:
	void HandleQueryFinished(TSharedPtr<struct FEnvQueryResult> Result);



protected:

	virtual void Tick(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable, Category="GameState")
	bool CheckAnyPlayerAlive();

	void StartWave(ASpawnTriggerBox* TargetTrigger);

protected:
	UWaveManager* WaveManager;
	
};
