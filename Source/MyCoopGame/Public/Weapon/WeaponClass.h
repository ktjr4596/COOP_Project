// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "MyCoopGame/ItemType.h"
#include "WeaponClass.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32, TargetAmmo);


UENUM(Blueprintable)
enum class EWeaponType :uint8
{
	WeaponType_Knife,
	WeaponType_Rifle,
	WeaponType_Count
};

USTRUCT(BlueprintType,Blueprintable)
struct FRecoilValues
{
	GENERATED_USTRUCT_BODY()

	FRecoilValues()
		:MinRecoilValue(0.0f)
		,MaxRecoilValue(0.0f)
	{
	}

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Recoil")
	float MinRecoilValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recoil")
	float MaxRecoilValue;

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
	virtual void Use(APlayerCharacter* OwningCharacter) override;

public:
	virtual void Start();
	virtual void Stop();

	virtual void ResetAmmo();
public:
	EWeaponType GetWeaponType() const;
	EAmmoType GetAmmoType() const;

protected:
	virtual void OnChangeState() override;
protected:
	UPROPERTY(BlueprintAssignable, Category="AmmoEvent")
	FOnAmmoChanged OnAmmoChange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Recoil")
	FRecoilValues RecoilValues;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ammo")
	EAmmoType AmmoType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float DamageBase;


};
