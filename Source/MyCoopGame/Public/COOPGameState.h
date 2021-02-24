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

	GameState_Clear,
};


UENUM(BlueprintType, Blueprintable)
enum class EGameProgress : uint8
{
	GameProgress_FindExit UMETA(DisplayName = "FindExit"),

	GameProgress_NeedTurnOnGenerator UMETA(DisplayName = "NeedTurnOnGenerator"),

	GameProgress_Exit UMETA(DisplayName = "Exit"),
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

	UFUNCTION(BlueprintCallable, Category="GameProgress")
	void SetGameProgress(EGameProgress NewProgress);
protected:

	UFUNCTION()
	void OnRep_GameStateChanged(EGameState OldState);

	UFUNCTION(BlueprintImplementableEvent, Category="GameState")
	void OnGameStateChanged(EGameState NewState,EGameState OldState);


	UFUNCTION()
	void OnRep_GameProgressChanged(EGameProgress OldProgress);

	UFUNCTION(BlueprintImplementableEvent, Category="GameProgress")
	void OnGameProgressChanged(EGameProgress NewProgress, EGameProgress OldProgress);

protected:
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_GameStateChanged, Category = "GameState")
	EGameState CurrentGameState;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing= OnRep_GameProgressChanged, Category="GameProgress")
	EGameProgress CurrentGameProgress;

};
