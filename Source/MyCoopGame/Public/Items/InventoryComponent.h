// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyCoopGame/ItemType.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryUpdated,class AActor*, OwningCharacter, class AItemBase* ,Item);


class AItemBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYCOOPGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItem(AItemBase* Item);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAddItem(AItemBase* Item);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveItem(AItemBase* Item, EItemState TargetState);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRemoveItem(AItemBase* Item, EItemState TargetState);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	int32 GetSize() const;

	UFUNCTION(BlueprintCallable, Category="InventoryUtil")
	TArray<AItemBase*> GetItemsByType(EItemType ItemType);

protected:
	int32* FindItemCountMap(int32 ItemID);

	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 GetItemCount(int32 ItemID);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory")
	TArray< AItemBase*> ItemArray;

	TMap<int32, int32> ItemCountMap;

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	UPROPERTY( BlueprintReadOnly,Category="Inventory")
	int32 CurrentItemCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 Capacity;

	
};
