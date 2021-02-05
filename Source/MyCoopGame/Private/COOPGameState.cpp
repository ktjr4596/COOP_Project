// Fill out your copyright notice in the Description page of Project Settings.


#include "COOPGameState.h"
#include "Net/UnrealNetwork.h"


void ACOOPGameState::SetGameState(EGameState NewState)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (CurrentGameState != NewState)
		{
			EGameState OldState = CurrentGameState;
			CurrentGameState = NewState;
			OnRep_GameStateChanged(OldState);
		}
	}
}

void ACOOPGameState::OnRep_GameStateChanged(EGameState OldState)
{

	OnGameStateChanged(CurrentGameState, OldState);
}


void ACOOPGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACOOPGameState, CurrentGameState);
}