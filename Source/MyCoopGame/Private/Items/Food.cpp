// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Food.h"
#include "kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "../MyCoopGame.h"
#include "Components/StaticMeshComponent.h"


AFood::AFood()
{

	PickUpMesh->SetCollisionResponseToChannel(COLLISION_INTERACT, ECollisionResponse::ECR_Block);

	SetReplicates(true);

}


void AFood::Use(APlayerCharacter* OwningCharacter)
{
	AActor* TargetActor = Cast<AActor>(OwningCharacter);
	if (nullptr != TargetActor)
	{
		RecoveryHealth *= -1.0f;
		UGameplayStatics::ApplyDamage(TargetActor, RecoveryHealth, nullptr, this, FoodType);
	}
}