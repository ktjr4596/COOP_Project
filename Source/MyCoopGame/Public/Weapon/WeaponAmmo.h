// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "MyCoopGame/MyCoopGame.h"
#include "WeaponAmmo.generated.h"



/**
 * 
 */
UCLASS()
class MYCOOPGAME_API AWeaponAmmo : public AItemBase
{
	GENERATED_BODY()
public:
	AWeaponAmmo();
public:
	EAmmoType GetAmmoType() const ;

	
protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Ammo")
	int32 AmmoCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ammo")
	EAmmoType AmmoType;


};
