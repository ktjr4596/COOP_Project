// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/InventoryComponent.h"
#include "Items/ItemBase.h"
#include "Net/UnrealNetwork.h"

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

	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerAddItem(Item);
	}

	if (ItemArray.Num() >= Capacity || nullptr == Item)
	{
		return false;
	}
	
	if (true == Item->CanOverlapped())
	{
		int32 ItemID = Item->GetItemID();
		int32* FindResult = FindItemCountMap(ItemID);
		if (nullptr != FindResult)
		{
			++(*FindResult);
		}
		else
		{
			ItemArray.Add(Item);
			ItemCountMap.Add(ItemID, 1);
		}
	}
	else
	{
		ItemArray.Add(Item);
	}
	Item->SetOwningInventory(this);
	Item->ChangeState(EItemState::ItemState_Inventory);
	OnInventoryUpdated.Broadcast(GetOwner(), Item);

	return true;
}

void UInventoryComponent::ServerAddItem_Implementation(AItemBase * Item)
{
	AddItem(Item);
}

bool UInventoryComponent::ServerAddItem_Validate(AItemBase* Item)
{
	return true;
}

bool UInventoryComponent::RemoveItem(AItemBase * Item, EItemState TargetState)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerRemoveItem(Item,TargetState);
	}
	if (nullptr != Item)
	{
		if (true == Item->CanOverlapped())
		{
			int32 ItemID = Item->GetItemID();
			int32* FindResult = FindItemCountMap(ItemID);
			if (nullptr != FindResult)
			{
				if (0 == (--(*FindResult)))
				{
					ItemCountMap.Remove(ItemID);
					ItemArray.Remove(Item);
				}
			}
			else
			{
				ItemArray.Remove(Item);
			}
		}
		else
		{
			ItemArray.Remove(Item);
		}
		Item->ResetOwningInvetory();     
		Item->ChangeState(TargetState);
		OnInventoryUpdated.Broadcast(GetOwner(),Item);

		return true;

	}
	return false;
}

void UInventoryComponent::ServerRemoveItem_Implementation(AItemBase* Item,EItemState TargetState)
{
	RemoveItem(Item,TargetState);
}

bool UInventoryComponent::ServerRemoveItem_Validate(AItemBase* Item,EItemState TargetState)
{
	return true;
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

int32 * UInventoryComponent::FindItemCountMap(int32 ItemID)
{
	return ItemCountMap.Find(ItemID);
}

int32 UInventoryComponent::GetItemCount(int32 ItemID)
{
	int32* FindResult = FindItemCountMap(ItemID);
	return (nullptr != FindResult ? *FindResult : AItemBase::ItemID_None);
}

