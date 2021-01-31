// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponClass.h"
#include "WeaponRifle.generated.h"


class UParticleSystem;

USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FVector_NetQuantize HitImpactPoint;

	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> HitSurfaceType;

	UPROPERTY()
	int8 Seed = 0;
};


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
	virtual void Start() override;
	virtual void Stop() override;
protected:
	void Fire();

	void StopFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

protected:
	void PlayFireEffect();
	void PlayImpactEffect(EPhysicalSurface SurfaceType, const FVector& ImpactPoint);
	void PlayMuzzleEffect();

	UFUNCTION()
	void OnRep_HitScanTrace();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


protected:

	FTimerHandle TimerHandle_Firing;

	UPROPERTY(ReplicatedUsing= OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

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
