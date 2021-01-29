// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class APlayerCharacter;
class UStaticMesh;
class UTexture2D;
class UInventoryComponent;

UENUM()
enum class EItemType
{
	ItemType_Cosumable,
	ItemType_Equipable,

};


UCLASS(Abstract,BlueprintType,Blueprintable)
class MYCOOPGAME_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

public:
	virtual void Use(APlayerCharacter* OwningCharacter) PURE_VIRTUAL(AItemBase, );

public:
	EItemType GetItemType() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FText ItemName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UStaticMesh* PickUpMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UTexture2D* ThumbnailImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	EItemType ItemType;

UPROPERTY()
	UInventoryComponent* OwningInventory;

};