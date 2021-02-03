// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/SpawnTriggerBox.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "COOPGameMode.h"

// Sets default values
ASpawnTriggerBox::ASpawnTriggerBox()
{

	TriggerBoxVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBoxVolume;

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
	}

}

void ASpawnTriggerBox::HandleOverlappedBox(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (nullptr != OtherActor)
	{
		APlayerCharacter* Player= Cast<APlayerCharacter>(OtherActor);
		if (nullptr != Player)
		{
			 AGameModeBase* GM= UGameplayStatics::GetGameMode(GetWorld());
			 ACOOPGameMode* CoopGM = Cast<ACOOPGameMode>(GM);
			 if (nullptr != CoopGM)
			 {
				 CoopGM->RunSpawnQuery(SpawningActor);
			 }
		}
	}
}

