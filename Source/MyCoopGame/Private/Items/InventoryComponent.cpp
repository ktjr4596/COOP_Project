// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/InventoryComponent.h"
#include "Items/ItemBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{

	Capacity = 20;

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
	ItemArray.Add(Item);
	Item->SetOwningInventory(this);
	Item->ChangeState(EItemState::ItemState_Inventory);
	OnInventoryUpdated.Broadcast();

	return true;
}

bool UInventoryComponent::RemoveItem(AItemBase * Item)
{
	if (nullptr != Item)
	{
		Item->ResetOwningInvetory();
		ItemArray.RemoveSingle(Item);
		OnInventoryUpdated.Broadcast();

		return true;

	}
	return false;
}

int32 UInventoryComponent::GetSize() const
{
	return ItemArray.Num();
}


