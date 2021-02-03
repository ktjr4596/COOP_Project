// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TestAI.generated.h"

class UStaticMeshComponent;
class UHealthComponent;

UCLASS()
class MYCOOPGAME_API ATestAI : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATestAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	FVector GetNextFindPath();

protected:
	void SelfDestruct();

	UFUNCTION()
	void HandleTakeDamage(class UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

protected:

	FTimerHandle TimerHandle_SelfDestruct;

	FVector NextPathPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Health")
	UHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Movement")
	float MovmentSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MinimumDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bUseVelocityChange;


	bool bIsExploded;
};
