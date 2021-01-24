// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponRifle.generated.h"

/**
 * 
 */
UCLASS()
class MYCOOPGAME_API AWeaponRifle : public AWeaponBase
{
	GENERATED_BODY()

public:
	AWeaponRifle();

public:
	virtual void Use() override final;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
