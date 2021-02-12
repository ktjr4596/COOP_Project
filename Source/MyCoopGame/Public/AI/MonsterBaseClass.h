// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIModule/Classes/GenericTeamAgentInterface.h"
#include "MonsterBaseClass.generated.h"


UENUM(BlueprintType)
enum class EMyMonsterState :uint8
{
	MonsterState_Idle UMETA(DisplayName="Idle"),

	MonsterState_Suspicous UMETA(DisplayName="Suspicous"),

	MonsterState_Chase UMETA(DisplayName="Chase"),

	MonsterState_Attack UMETA(DisplayName="Attack"),

	MonsterState_Dead UMETA(DisplayName="Dead"),
};

class UCapsuleComponent;
class USkeletalMeshComponent;
class UHealthComponent;
class UBehaviorTree;
class UAIPerceptionComponent;

UCLASS()
class MYCOOPGAME_API AMonsterBaseClass : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonsterBaseClass();

public:
	virtual void Tick(float DeltaSeconds)override;

	virtual void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Call for initializing blackboard's data in blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "BlackBoard")
	void SetupInitialize();

	// Called when health changed
	UFUNCTION()
	void HandleHealthChanged(UHealthComponent * TargetHealthComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser);

	UFUNCTION(BlueprintCallable, Category="Target")
	void ResetTargetByTimer();

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent, Category="Target")
	void ResetTarget();

private:
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Team")
	FGenericTeamId TeamID;

	FTimerHandle TimerHandleForResetTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	UHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	UBehaviorTree*	BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception")
	UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY(BlueprintReadWrite, Category="Attack")
	class AActor* TargetActor;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Target")
	float AttackDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Target")
	float TimeForResetTarget;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	EMyMonsterState MonsterState;

	UPROPERTY(BlueprintReadOnly ,Category="State")
	bool bIsDied;


};
