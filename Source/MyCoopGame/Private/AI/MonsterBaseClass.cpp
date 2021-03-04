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
#include "Net/UnrealNetwork.h"
#include "COOPGameMode.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "DrawDebugHelpers.h"
#include "Items/ItemBase.h"

// Sets default values
AMonsterBaseClass::AMonsterBaseClass()
{

	PrimaryActorTick.bCanEverTick = true;


	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("SensingComp"));
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	MonsterState = EMyMonsterState::MonsterState_Idle;

	SetReplicates(true);
}

void AMonsterBaseClass::HandleQueryFinished(TSharedPtr<struct FEnvQueryResult> Result)
{
	if (true == Result->IsSuccsessful())
	{
		TArray<FVector> OutLocations;
		Result->GetAllAsLocations(OutLocations);

		if (0 < DropItemArray.Num())
		{
			int32 SelectedIndex= FMath::RandRange(0, DropItemArray.Num() - 1);

			GetWorld()->SpawnActor(DropItemArray[SelectedIndex], &OutLocations[0]);
		}
	}
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
		OnRepMonsterDied();

		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);
	}
}

void AMonsterBaseClass::ResetTargetByTimer()
{
	GetWorldTimerManager().ClearTimer(TimerHandleForResetTarget);
	GetWorldTimerManager().SetTimer(TimerHandleForResetTarget, this, &AMonsterBaseClass::ResetTarget, TimeForResetTarget, false);
}


void AMonsterBaseClass::OnRepMonsterDied()
{
	MonsterState = EMyMonsterState::MonsterState_Dead;

	FEnvQueryRequest QueryRequest = FEnvQueryRequest(DropItemQuery, this);

	QueryRequest.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &AMonsterBaseClass::HandleQueryFinished);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

void AMonsterBaseClass::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMonsterBaseClass, bIsDied);
	DOREPLIFETIME(AMonsterBaseClass, MonsterState);
	DOREPLIFETIME(AMonsterBaseClass, bIsAttacking);
	DOREPLIFETIME(AMonsterBaseClass, AttackMotionType);
	DOREPLIFETIME(AMonsterBaseClass, TargetActor);

}