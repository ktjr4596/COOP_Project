// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UENUM(Blueprintable)
enum class EWeaponType :uint8
{
	WeaponType_Knife,
	WeaponType_Rifle,
	WeaponType_Count
};


UCLASS(Abstract)
class MYCOOPGAME_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Use() PURE_VIRTUAL(AWeaponBase::Use, ;);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly , Category= "Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	EWeaponType WeaponType;
};
