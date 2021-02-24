// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter.h"
#include "Items/InventoryComponent.h"
#include "../MyCoopGame.h"


const int32  AItemBase::ItemID_None = 255;

// Sets default values

AItemBase::AItemBase()
{
	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	RootComponent = PickUpMesh;

	bCanOverlapped = false;
	PickUpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetReplicates(true);

	PickUpMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickUpMesh->SetCollisionResponseToChannel(COLLISION_ITEM, ECollisionResponse::ECR_Block);
	PickUpMesh->SetCollisionObjectType(COLLISION_ITEM);

	PickUpMesh->ComponentTags.Add(FName("outline"));

}

void AItemBase::Use(APlayerCharacter * OwningCharacter)
{
}

void AItemBase::Interact_Implementation(APlayerCharacter * Character)
{
	Super::Interact_Implementation(Character);
	if (nullptr != Character)
	{
		UInventoryComponent* InventoryComp= Character->GetInventory();
		if (nullptr != InventoryComp)
		{
			InventoryComp->AddItem(this);
		}
	}
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

		OnRep_ChangeState();
	}
}

void AItemBase::ServerChangeState_Implementation(EItemState NewState)
{
	ChangeState(NewState);
}

bool AItemBase::ServerChangeState_Validate(EItemState NewState)
{
	return true;
}

bool AItemBase::CanOverlapped()
{
	return bCanOverlapped;
}

int32 AItemBase::GetItemID()
{
	return ItemID;
}

void AItemBase::SetHiddenPickupMesh(bool isHideMesh)
{
	PickUpMesh->SetHiddenInGame(isHideMesh);
	PickUpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AItemBase::OnChangeState()
{
	
	switch (ItemState)
	{
	case EItemState::ItemState_None:
		break;
	case EItemState::ItemState_Field:
		PickUpMesh->SetHiddenInGame(false);
		PickUpMesh->SetCollisionResponseToChannel(COLLISION_INTERACT,ECollisionResponse::ECR_Block);
		PickUpMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case EItemState::ItemState_Inventory:
		PickUpMesh->SetHiddenInGame(true);
		PickUpMesh->SetCollisionResponseToChannel(COLLISION_INTERACT, ECollisionResponse::ECR_Ignore);
		PickUpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EItemState::ItemState_Equip:
		break;
	case EItemState::ItemState_Key:
		break;
	default:
		break;
	}
}

void AItemBase::OnRep_ChangeState()
{
	OnChangeState();
}

int32 AItemBase::GetItemID_None()
{
	return ItemID_None;
}


// Relicated로 선언된 변수들을 엔진 쪽에서 인식할 수 있는 방법
void AItemBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemBase, ItemState);
}