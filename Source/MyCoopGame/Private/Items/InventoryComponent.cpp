// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/InventoryComponent.h"
#include "Items/ItemBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{

	Capacity = 20;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UInventoryComponent::AddItem(AItemBase * Item)
{
	if (ItemArray.Num() >= Capacity || nullptr == Item)
	{
		return false;
	}
	if (true == Item->CanOverlapped())
	{

	}
	ItemArray.Add( Item);
	Item->SetOwningInventory(this);
	Item->ChangeState(EItemState::ItemState_Inventory);
	OnInventoryUpdated.Broadcast(GetOwner(), Item);

	return true;
}

bool UInventoryComponent::RemoveItem(AItemBase * Item)
{
	if (nullptr != Item)
	{
		Item->ResetOwningInvetory();     
		ItemArray.Remove(Item);
		OnInventoryUpdated.Broadcast(GetOwner(),Item);

		return true;

	}
	return false;
}

int32 UInventoryComponent::GetSize() const
{
	return ItemArray.Num();
}

TArray<AItemBase*> UInventoryComponent::GetItemsByType(EItemType ItemType)
{
	TArray<AItemBase*> Result;
	for (int32 ii = 0; ii < ItemArray.Num(); ++ii)
	{
		if (ItemArray[ii]->GetItemType() == ItemType)
		{
			Result.Add( ItemArray[ii]);
		}
	}

	return Result;
}


