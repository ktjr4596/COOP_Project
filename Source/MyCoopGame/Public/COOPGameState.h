// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "COOPGameState.generated.h"

UENUM(BlueprintType)
enum class EGameState:uint8
{
	GameState_Progress,

	GameState_GameOver,
};

/**
 * 
 */
UCLASS()
class MYCOOPGAME_API ACOOPGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category="GameState")
	void SetGameState(EGameState NewState);
protected:

	UFUNCTION()
	void OnRep_GameStateChanged(EGameState OldState);

	UFUNCTION(BlueprintImplementableEvent, Category="GameState")
	void OnGameStateChanged(EGameState NewState,EGameState OldState);
protected:
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_GameStateChanged, Category = "GameState")
	EGameState CurrentGameState;
};
