// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCoopGame/Public/Items/ItemBase.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

}

EItemType AItemBase::GetItemType() const
{
	return ItemType;
}
