// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/ExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "MyCoopGame/MyCoopGame.h"
#include "Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionResponseToChannel(COLLISION_WEAPON,ECollisionResponse::ECR_Block);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->bIgnoreOwningActor = true;
	bIsExploded = false;
	
	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetLocalRole() == ROLE_Authority)
	{
		if (nullptr != HealthComp)
		{
			HealthComp->OnHealthChanged.AddDynamic(this, &AExplosiveBarrel::HandleTakeAnyDamage);
		}

	}


}

void AExplosiveBarrel::HandleTakeAnyDamage(UHealthComponent * OwningHealthComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (true == bIsExploded)
	{
		return;
	}

	if (Health <= 0.0f)
	{
		bIsExploded = true;

		OnRep_Explosion();
	}

}

void AExplosiveBarrel::Explode()
{
	if (nullptr != ExplosivedMaterial)
	{
		MeshComp->SetMaterial(0, ExplosivedMaterial);
	}
	PlayExplosionEffect();
	FVector UpVector = GetActorUpVector();
	UpVector.Normalize();
	MeshComp->AddImpulse(UpVector*1000.0f,NAME_None,true);
	RadialForceComp->FireImpulse();

}

void AExplosiveBarrel::PlayExplosionEffect()
{
	if (nullptr != ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	}
}

void AExplosiveBarrel::OnRep_Explosion()
{
	Explode();
}



void AExplosiveBarrel::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExplosiveBarrel, bIsExploded);
}