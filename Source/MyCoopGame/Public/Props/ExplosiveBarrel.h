// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBarrel.generated.h"


class UStaticMeshComponent;
class UParticleSystem;
class UHealthComponent;
class UMaterialInterface;
class URadialForceComponent;

UCLASS()
class MYCOOPGAME_API AExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleTakeAnyDamage(class UHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void Explode();
	void PlayExplosionEffect();

	UFUNCTION()
	void OnRep_Explosion();
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
	UHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FX")
	UMaterialInterface* ExplosivedMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Force")
	URadialForceComponent* RadialForceComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Force")
	float ImpluseStrenth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Explosion,Category="Health")
	bool bIsExploded;
};
