// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactive/InteractiveActorBase.h"
#include "Components/SceneComponent.h"

// Sets default values
AInteractiveActorBase::AInteractiveActorBase()
	:bIsStartInteract(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	ActionName= FText::FromName("Interact");

	SetReplicates(true);
}

void AInteractiveActorBase::Interact_Implementation(APlayerCharacter * Charcater)
{
}

void AInteractiveActorBase::BroadCastNotify()
{
	OnInteract.Broadcast(this);
}

