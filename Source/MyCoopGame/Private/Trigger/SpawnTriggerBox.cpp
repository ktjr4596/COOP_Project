// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/SpawnTriggerBox.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "COOPGameMode.h"
#include "Trigger/TriggeredActor.h"
#include "AI/MonsterBaseClass.h"
#include "Engine/World.h"

// Sets default values
ASpawnTriggerBox::ASpawnTriggerBox()
{

	TriggerBoxVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBoxVolume;

	bIsTriggered = false;
}

UEnvQuery * ASpawnTriggerBox::GetEnvQuery()
{
	return TargetEnvQuery;
}

UEnvQuery * ASpawnTriggerBox::GetSpawnedActorTargetEnvQuery()
{
	return SpawnedTargetEnvQuery;
}

void ASpawnTriggerBox::HandleStartWave(ASpawnTriggerBox* CurrentTrigger)
{
	if (this != CurrentTrigger)
	{
		return;
	}
	OnTriggerOverlapped.Broadcast(true);
}

void ASpawnTriggerBox::HandleEndWave(ASpawnTriggerBox* CurrentTrigger)
{
	if (this != CurrentTrigger)
	{
		return;
	}
	OnTriggerOverlapped.Broadcast(false);
}

ATargetPoint * ASpawnTriggerBox::GetSpawendActorTarget()
{
	return SpawnedActorTarget;
}

TArray<ATriggeredActor*> ASpawnTriggerBox::GetTriggeredActors()
{
	return TriggeredActors;
}

AMonsterBaseClass* ASpawnTriggerBox::SpawnMonster(const FVector & TargetLocation)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	return Cast<AMonsterBaseClass>(GetWorld()->SpawnActor(SpawningActor[0], &TargetLocation, &FRotator::ZeroRotator, SpawnParams));
}

// Called when the game starts or when spawned
void ASpawnTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		if (nullptr != TriggerBoxVolume)
		{
			TriggerBoxVolume->OnComponentBeginOverlap.AddDynamic(this, &ASpawnTriggerBox::HandleOverlappedBox);
		}

		for (int32 ii = 0; ii < TriggeredActors.Num(); ++ii)
		{
			OnTriggerOverlapped.AddDynamic(TriggeredActors[ii], &ATriggeredActor::ActivateByTriggered);
		}
	}

}

void ASpawnTriggerBox::HandleOverlappedBox(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (true == bIsTriggered)
	{
		return;
	}
	if (nullptr != OtherActor)
	{
		APlayerCharacter* Player= Cast<APlayerCharacter>(OtherActor);
		if (nullptr != Player)
		{
			 AGameModeBase* GM= UGameplayStatics::GetGameMode(GetWorld());
			 ACOOPGameMode* CoopGM = Cast<ACOOPGameMode>(GM);
			 if (nullptr != CoopGM)
			 {
				 //FWaveParams WaveParams;
				 //WaveParams.SpanwedPoints = SpanwedPoints;
				 //
				 //for (int32 ii = 0; ii < SpawningActor.Num(); ++ii)
				 //{
					// WaveParams.SpawningActors.Add(SpawningActor[ii]->StaticClass());
				 //}
				 //WaveParams.TargetEnvQuery = TargetEnvQuery;
				 CoopGM->StartWave(this);
				 //CoopGM->RunSpawnQuery(this,SpawningActor);
			 }
		}
	}
	bIsTriggered = true;
}

