// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCoopGame/Public/Items/ItemBase.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AItemBase::AItemBase()
{
	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	RootComponent = PickUpMesh;

}

void AItemBase::Use(APlayerCharacter * OwningCharacter)
{
}

EItemType AItemBase::GetItemType() const
{
	return ItemType;
}

void AItemBase::SetOwningInventory(UInventoryComponent * TargetInventory)
{
	if (nullptr != TargetInventory)
	{
		OwningInventory = TargetInventory;
	}
}

void AItemBase::ResetOwningInvetory()
{
	OwningInventory = nullptr;
}

void AItemBase::ChangeState(EItemState NewState)
{
	if (NewState != ItemState)
	{
		ItemState = NewState;

		OnChangeState();
	}
}

void AItemBase::SetHiddenPickupMesh(bool isHideMesh)
{
	PickUpMesh->SetHiddenInGame(isHideMesh);
	PickUpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AItemBase::OnChangeState()
{
}
