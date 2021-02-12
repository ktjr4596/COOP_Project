// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MonsterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Perception/AIPerceptionComponent.h"

// Sets default values
AMonsterBase::AMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	RootComponent = CapsuleComp;
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	MeshComp->SetupAttachment(CapsuleComp);

	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("SensingComp"));
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	PawnMovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComp"));

	PawnMovementComp->MaxSpeed = 500.0f;


	SetReplicates(true);
}

void AMonsterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


}

void AMonsterBase::GetActorEyesViewPoint(FVector & Location, FRotator & Rotation) const
{
	Location = MeshComp->GetSocketLocation(TEXT("head"));

	Rotation = GetActorRotation();
	//Rotation.Yaw -= MeshComp->GetSocketTransform(TEXT("hhead"), ERelativeTransformSpace::RTS_ParentBoneSpace).Rotator().Roll;

}


// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (ROLE_Authority == GetLocalRole())
	{
		AAIController* AIController = Cast<AAIController>(GetController());

		if (nullptr != AIController && nullptr != BehaviorTree)
		{
			AIController->RunBehaviorTree(BehaviorTree);

			SetupInitialize();
			//AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this,&)
		}
	}
}


