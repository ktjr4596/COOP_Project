// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCoopGame/Public/Items/InventoryComponent.h"

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

bool UInventoryComponent::AddItem(AItemBase * item)
{
	return false;
}

bool UInventoryComponent::RemoveItem(AItemBase * item)
{
	return false;
}


