// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponRifle.generated.h"


class UParticleSystem;

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
	void PlayFireEffect();
	void PlayMuzzleEffect();
	void PlayTraceEffect();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, Category="FX")
	FName TracerEffectPointName;

	UPROPERTY(EditDefaultsOnly, Category="FX")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category="FX")
	UParticleSystem* TracerEffect;

};
