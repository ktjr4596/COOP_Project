// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TestAI.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "GameFramework/Character.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "DrawDebugHelpers.h"
#include "Components/HealthComponent.h"

// Sets default values
ATestAI::ATestAI()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);

	RootComponent = MeshComp;
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

}

// Called when the game starts or when spawned
void ATestAI::BeginPlay()
{
	Super::BeginPlay();
	
	NextPathPoint= GetNextFindPath();
	
	if (GetLocalRole() == ROLE_Authority)
	{
		if (nullptr != HealthComp)
		{
			HealthComp->OnHealthChanged.AddDynamic(this, &ATestAI::HandleTakeDamage);
		}
	}

}

// Called every frame
void ATestAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetLocalRole() == ROLE_Authority)
	{
		float DistanceToTarget = (NextPathPoint - GetActorLocation()).Size();
		if (MinimumDistanceToTarget < DistanceToTarget)
		{
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();

			MeshComp->AddForce(ForceDirection*MovmentSpeed, NAME_None, bUseVelocityChange);

			DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ForceDirection, 32, FColor::Yellow, false, 0.0f, 0, 1.0f);

		}
		else
		{
			DrawDebugString(GetWorld(), GetActorLocation(), TEXT("Target Reached"));
			NextPathPoint = GetNextFindPath();
			DrawDebugSphere(GetWorld(), NextPathPoint, 32.f, 12, FColor::Yellow, false, 3.0f, 0, 2.0f);
		}
	}
}

FVector ATestAI::GetNextFindPath()
{

	ACharacter* PlayerPawn= UGameplayStatics::GetPlayerCharacter(this, 0);
	if (nullptr != PlayerPawn)
	{
		UNavigationPath* NavPath= UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);
		if (nullptr != NavPath)
		{
			if (NavPath->PathPoints.Num() > 1)
			{
				return NavPath->PathPoints[1];
			}
		}
	}

	return GetActorLocation();
}

void ATestAI::SelfDestruct()
{
	if (true == bIsExploded)
	{
		return;
	}

	bIsExploded = true;

	MeshComp->SetVisibility(false, true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DrawDebugSphere(GetWorld(), GetActorLocation(), 200.0f, 12, FColor::Red, false, 3.0f, 0, 1.0f);

	SetLifeSpan(3.0f);

}

void ATestAI::HandleTakeDamage(UHealthComponent * OwningHealthComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (true == bIsExploded)
	{
		return;
	}

	GetWorldTimerManager().SetTimer(TimerHandle_SelfDestruct, this, &ATestAI::SelfDestruct, 3.0f);

}


