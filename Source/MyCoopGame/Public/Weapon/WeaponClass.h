// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "WeaponClass.generated.h"




UENUM(Blueprintable)
enum class EWeaponType :uint8
{
	WeaponType_Knife,
	WeaponType_Rifle,
	WeaponType_Count
};



/**
 * 
 */
UCLASS()
class MYCOOPGAME_API AWeaponClass : public AItemBase
{
	GENERATED_BODY()
public:
	AWeaponClass();

public:
	EWeaponType GetWeaponType() const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float DamageBase;

	

};
