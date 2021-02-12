// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnTriggerBox.generated.h"


class UBoxComponent;
class UEnvQuery;

UCLASS()
class MYCOOPGAME_API ASpawnTriggerBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnTriggerBox();

public:
	UEnvQuery* GetEnvQuery();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void HandleOverlappedBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "SpanwedActor")
	TSubclassOf<class AActor> SpawningActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TriggerVolume")
	UBoxComponent* TriggerBoxVolume;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "SpawnedPoints")
	TArray<class ATargetPoint*> SpanwedPoints;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "EnvQuery")
	UEnvQuery * TargetEnvQuery;

};
