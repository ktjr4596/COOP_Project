// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MonsterBase.generated.h"

//UENUM(BlueprintType)
//enum class EMonsterState :uint8
//{
//	MonsterState_Normal,
//
//	MonsterState_Suspicous,
//
//	MonsterState_Alert,
//
//	MonsterState_Dead,
//};

class UCapsuleComponent;
class USkeletalMeshComponent;
class UHealthComponent;
class UFloatingPawnMovement;
class UBehaviorTree;
class UAIPerceptionComponent;

UCLASS(Abstract,BlueprintType,Blueprintable)
class MYCOOPGAME_API AMonsterBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMonsterBase();

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

	void HandlePerceptionTargetUpdate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category="BlackBoard")
	void SetupInitialize();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	UHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UFloatingPawnMovement* PawnMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	UBehaviorTree*	BehaviorTree;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Perception")
	UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Attack")
	float AttackDistance;

	//UPROPERTY(BlueprintReadOnly, Category = "State")
	//EMonsterState MonsterState;

};
