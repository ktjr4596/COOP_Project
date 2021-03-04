// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Food.h"
#include "kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "../MyCoopGame.h"
#include "Components/StaticMeshComponent.h"
#include "Items/InventoryComponent.h"

AFood::AFood()
{

	PickUpMesh->SetCollisionResponseToChannel(COLLISION_INTERACT, ECollisionResponse::ECR_Block);

	SetReplicates(true);

}


void AFood::Use(APlayerCharacter* OwningCharacter)
{
	if (nullptr != OwningCharacter)
	{
		RecoveryHealth *= -1.0f;
		UGameplayStatics::ApplyDamage(OwningCharacter, RecoveryHealth, nullptr, this, FoodType);

		UInventoryComponent* InventoryComp= OwningCharacter->GetInventory();
		if (nullptr != InventoryComp)
		{
			InventoryComp->RemoveItem(this, EItemState::ItemState_None);
		}
	}
}