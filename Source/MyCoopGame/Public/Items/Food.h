// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "Food.generated.h"

/**
 * 
 */
UCLASS()
class MYCOOPGAME_API AFood : public AItemBase
{
	GENERATED_BODY()
public:
	AFood();
public:
	virtual void Use(APlayerCharacter* OwningCharacter);

protected:
	UPROPERTY(EditDefaultsOnly, Category="Food")
	float RecoveryHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Food")
	TSubclassOf<class UDamageType> FoodType;
};
