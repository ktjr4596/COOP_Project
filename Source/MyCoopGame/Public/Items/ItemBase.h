// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactive/InteractiveActorBase.h"
#include "MyCoopGame/ItemType.h"
#include "ItemBase.generated.h"

class APlayerCharacter;
class UStaticMeshComponent;
class UTexture2D;
class UInventoryComponent;

UENUM(BlueprintType)
enum class EItemState : uint8
{
	ItemState_None UMETA(DisplayName="None"),

	ItemState_Field UMETA(DisplayName="Field"),

	ItemState_Inventory UMETA(DisplayName="Inventory"),

	ItemState_Equip UMETA(DisplayName="Equip"),

	ItemState_Key UMETA(DisplayName="Key"),
};


UCLASS(Abstract,BlueprintType,Blueprintable)
class MYCOOPGAME_API AItemBase : public AInteractiveActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

public:
	virtual void Use(APlayerCharacter* OwningCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(APlayerCharacter* OwningCharacter);

	virtual void Interact_Implementation(APlayerCharacter* Character) override;

public:
	EItemType GetItemType() const;
public:
	void SetOwningInventory(UInventoryComponent* TargetInventory);
	void ResetOwningInvetory();
public:
	UFUNCTION(BlueprintCallable ,Category="Item")
	void ChangeState(EItemState NewState);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerChangeState(EItemState NewState);

	bool CanOverlapped();

	int32 GetItemID();
protected:
	void SetHiddenPickupMesh(bool isHideMesh);
	
	virtual void OnChangeState();

	UFUNCTION()
	void OnRep_ChangeState();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UStaticMeshComponent * PickUpMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UTexture2D* ThumbnailImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	EItemType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, ReplicatedUsing= OnRep_ChangeState, Category = "Item")
	EItemState ItemState;

	UPROPERTY(BlueprintReadOnly, Category="Item")
	UInventoryComponent* OwningInventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	int32 ItemID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	bool bCanOverlapped;

public:
	UFUNCTION(BlueprintPure, Category="Item")
	static int32 GetItemID_None();

	static const int32 ItemID_None;

};