// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggeredActor.generated.h"

UCLASS(Abstract,BlueprintType,Blueprintable)
class MYCOOPGAME_API ATriggeredActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggeredActor();

public:
	UFUNCTION(BlueprintNativeEvent, Category = "TriggeredActions")
	void ActivateByTriggered(bool isStart);

	virtual void ActivateByTriggered_Implementation(bool isStart);
};
