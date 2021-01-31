// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AItemBase::AItemBase()
{
	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	RootComponent = PickUpMesh;

	SetReplicates(true);

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

void AItemBase::OnRep_ChangeState()
{
	OnChangeState();
}


// Relicated로 선언된 변수들을 엔진 쪽에서 인식할 수 있는 방법
void AItemBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AItemBase, ItemState,COND_SkipOwner);
}