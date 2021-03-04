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
		TArray<AItemBase*>* FindResult = FindItemCountMap(ItemID);
		if (nullptr != FindResult)
		{
			FindResult->Add(Item);
		}
		else
		{
			ItemArray.Add(Item);
			TArray<AItemBase*> NewItemArray;
			ItemCountMap.Add(ItemID, NewItemArray);
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
		Item->ResetOwningInvetory();
		Item->ChangeState(TargetState);
		if (true == Item->CanOverlapped())
		{
			int32 ItemID = Item->GetItemID();
			TArray<AItemBase*>* FindResult = FindItemCountMap(ItemID);
			if (nullptr != FindResult)
			{
				if (0 == FindResult->Num())
				{
					ItemCountMap.Remove(ItemID);
				}
				else 
				{
					AItemBase* LastItem = FindResult->Pop();
					ItemArray.Add(LastItem);
					int32 ItemIndex= ItemArray.IndexOfByKey(Item);
					if (INDEX_NONE != ItemIndex)
					{
						ItemArray.Swap(ItemIndex, ItemArray.Num()-1);
						ItemArray.Pop();
					}
					OnInventoryUpdated.Broadcast(GetOwner(), Item);
					return true;
				}
			}
		}
		ItemArray.Remove(Item);
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

TArray<AItemBase*>* UInventoryComponent::FindItemCountMap(int32 ItemID)
{
	return ItemCountMap.Find(ItemID);
}

int32 UInventoryComponent::GetItemCount(int32 ItemID)
{
	const TArray<AItemBase*>* FindResult = FindItemCountMap(ItemID);
	return (nullptr != FindResult ? FindResult->Num()+1 : AItemBase::ItemID_None);
}

