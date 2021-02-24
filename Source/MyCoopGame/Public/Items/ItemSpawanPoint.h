// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "ItemSpawanPoint.generated.h"

class AItemBase;

UCLASS()
class MYCOOPGAME_API AItemSpawanPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawanPoint();

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="SpawningItem")
	TArray<TSubclassOf<AItemBase>> SpawningItems;
};
