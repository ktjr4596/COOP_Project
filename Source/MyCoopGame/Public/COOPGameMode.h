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
	ACOOPGameMode();

protected:

	virtual void Tick(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable, Category="GameState")
	bool CheckAnyPlayerAlive();

};
