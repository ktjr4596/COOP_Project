// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class APlayerCharacter;
class UStaticMeshComponent;
class UTexture2D;
class UInventoryComponent;

UENUM()
enum class EItemType
{
	ItemType_Cosumable,
	ItemType_Equipable,
};

UENUM()
enum class EItemState
{
	ItemState_Field,
	ItemState_Inventory,
	ItemState_Equip,
};


UCLASS(Abstract,BlueprintType,Blueprintable)
class MYCOOPGAME_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

public:
	virtual void Use(APlayerCharacter* OwningCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(APlayerCharacter* OwningCharacter);

public:
	EItemType GetItemType() const;
public:
	void SetOwningInventory(UInventoryComponent* TargetInventory);
	void ResetOwningInvetory();
public:
	void ChangeState(EItemState NewState);
protected:
	void SetHiddenPickupMesh(bool isHideMesh);
	
	virtual void OnChangeState();

	UFUNCTION()
	void OnRep_ChangeState();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FText ItemName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UStaticMeshComponent * PickUpMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UTexture2D* ThumbnailImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	EItemType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing=OnRep_ChangeState, Category = "Item")
	EItemState ItemState;

	UPROPERTY()
	UInventoryComponent* OwningInventory;

};