// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"


class UTexture2D;
class UStaticMesh;
class UInventoryComponent;
class APlayerCharacter;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class MYCOOPGAME_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	UItemBase();

public:
	virtual void Use(APlayerCharacter* OwningCharacter) PURE_VIRTUAL(UItemBase, );

	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(APlayerCharacter* OwningCharacter);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item")
	FText UseActionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item")
	FText ItemDisplayName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true))
	FText ItemDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	UStaticMesh* PickUpMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	UTexture2D* ThumbnailImage;

	UPROPERTY()
	UInventoryComponent* OwningInventory;

};
