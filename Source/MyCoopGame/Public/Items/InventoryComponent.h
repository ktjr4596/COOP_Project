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
	
	bool AddItem(AItemBase* Item);
	bool RemoveItem(AItemBase* Item);

	int32 GetSize() const;

	UFUNCTION(BlueprintCallable, Category="InventoryUtil")
	TArray<AItemBase*> GetItemsByType(EItemType ItemType);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory")
	TArray< AItemBase*> ItemArray;

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	UPROPERTY( BlueprintReadOnly,Category="Inventory")
	int32 CurrentItemCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 Capacity;


	
};
