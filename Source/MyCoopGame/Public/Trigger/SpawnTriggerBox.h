// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnTriggerBox.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggerOverlapped,bool, isStart);


class AActor;
class UBoxComponent;
class UEnvQuery;
class ATargetPoint;
class ATriggeredActor;
class AMonsterBaseClass;

UCLASS()
class MYCOOPGAME_API ASpawnTriggerBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnTriggerBox();

public:
	UEnvQuery* GetEnvQuery();

	UEnvQuery* GetSpawnedActorTargetEnvQuery();

	UFUNCTION()
	void HandleStartWave();

	UFUNCTION()
	void HandleEndWave();

	// Get target point spawned actor going to
	ATargetPoint* GetSpawendActorTarget();

	TArray<ATriggeredActor*> GetTriggeredActors();

	AMonsterBaseClass* SpawnMonster(const FVector& TargetLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void HandleOverlappedBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:

	UPROPERTY(BlueprintAssignable, Category="TriggerEvnets")
	FOnTriggerOverlapped OnTriggerOverlapped;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "SpanwedActor")
	TArray<TSubclassOf<AActor>> SpawningActor;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "SpawnedPoints")
	TArray<ATargetPoint*> SpanwedPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TriggerVolume")
	UBoxComponent* TriggerBoxVolume;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "EnvQuery")
	UEnvQuery * TargetEnvQuery;

	UPROPERTY(EditInstanceOnly,BlueprintReadOnly, Category="Target")
	ATargetPoint* SpawnedActorTarget;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="EnvQuery")
	UEnvQuery* SpawnedTargetEnvQuery;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Trigger")
	TArray< ATriggeredActor*> TriggeredActors;

	bool bIsTriggered;

};
