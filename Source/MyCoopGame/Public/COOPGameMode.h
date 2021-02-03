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

};
