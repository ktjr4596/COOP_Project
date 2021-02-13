// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MonsterBaseClass.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "TimerManager.h"

// Sets default values
AMonsterBaseClass::AMonsterBaseClass()
{

	PrimaryActorTick.bCanEverTick = true;


	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("SensingComp"));
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	MonsterState = EMyMonsterState::MonsterState_Idle;

	SetReplicates(true);
}

void AMonsterBaseClass::ActivateActionByWave_Implementation(UEnvQuery* ActivateQuery, ATargetPoint* TargetPoint)
{
}

// Called when the game starts or when spawned
void AMonsterBaseClass::BeginPlay()
{
	Super::BeginPlay();

	if (ROLE_Authority == GetLocalRole())
	{
		AAIController* AIController = Cast<AAIController>(GetController());

		if (nullptr != AIController && nullptr != BehaviorTree)
		{
			AIController->RunBehaviorTree(BehaviorTree);

			HealthComp->OnHealthChanged.AddDynamic(this, &AMonsterBaseClass::HandleHealthChanged);

			SetupInitialize();
		}
	}
	
}
void AMonsterBaseClass::HandleHealthChanged(UHealthComponent * TargetHealthComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (true==bIsDied)
	{
		return;
	}
	if (Health <= 0.0f)
	{
		bIsDied = true;

		MonsterState = EMyMonsterState::MonsterState_Dead;

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetLifeSpan(10.0f);
	}
}

void AMonsterBaseClass::ResetTargetByTimer()
{
	GetWorldTimerManager().ClearTimer(TimerHandleForResetTarget);
	GetWorldTimerManager().SetTimer(TimerHandleForResetTarget, this, &AMonsterBaseClass::ResetTarget, TimeForResetTarget, false);
}


FGenericTeamId AMonsterBaseClass::GetGenericTeamId() const
{
	return TeamID;
}

void AMonsterBaseClass::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


void AMonsterBaseClass::GetActorEyesViewPoint(FVector & Location, FRotator & Rotation) const
{
	Location = GetMesh()->GetSocketLocation(TEXT("head"));

	Rotation = GetActorRotation();
}

bool AMonsterBaseClass::IsDied()
{
	return bIsDied;
}

