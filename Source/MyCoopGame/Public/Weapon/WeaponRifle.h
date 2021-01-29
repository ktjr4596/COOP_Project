// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponClass.h"
#include "WeaponRifle.generated.h"


class UParticleSystem;

/**
 * 
 */
UCLASS()
class MYCOOPGAME_API AWeaponRifle : public AWeaponClass
{
	GENERATED_BODY()

public:
	AWeaponRifle();

public:
	virtual void Use(APlayerCharacter* OwningCharacter) override final;

protected:
	void Fire();

	void StopFire();

protected:
	void PlayFireEffect();

	void PlayImpactEffect(EPhysicalSurface SurfaceType, const FVector& ImpactPoint, const FVector& ImpactNormal);
	void PlayMuzzleEffect();
	void PlayTraceEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


protected:

	FTimerHandle TimerHandle_Firing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, Category="FX")
	FName TracerEffectPointName;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShake> FireCamShake;

	UPROPERTY(EditDefaultsOnly, Category="FX")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category="FX")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category="FleshFX")
	UParticleSystem* FleshEffect;

	/** Firing Ammo per minute */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ammo")
	float RateOfFire;

	float LastFireTime;

	float TimeBetweenShots;
};
