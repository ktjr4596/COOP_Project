// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "COOPGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYCOOPGAME_API ACOOPGameMode : public AGameModeBase
{
	GENERATED_BODY()
<<<<<<< HEAD
	
public:
	void RunSpawnQuery(class UClass* SpawningActor);

protected:
	virtual void BeginPlay() override;
	
protected:
	void HandleQueryFinished(TSharedPtr<struct FEnvQueryResult> Result);

protected:

	TArray<class UClass*> SpawningActors;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EnvQuery")
	class UEnvQuery* TargetEnvQuery;
=======

public:
	ACOOPGameMode();

protected:

	virtual void Tick(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable, Category="GameState")
	bool CheckAnyPlayerAlive();
>>>>>>> 930afb352dbb6db38559785bb5c3a130b7016ffa

};
