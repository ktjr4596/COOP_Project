// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponRifle.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "../MyCoopGame.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(TEXT("COOP.DebugWeapons"), DebugWeaponDrawing, TEXT("Draw Debug Lines for weapons"), ECVF_Cheat);

AWeaponRifle::AWeaponRifle()
	:AWeaponBase()
{

}

void AWeaponRifle::Use()
{
	AActor* MyOwner = GetOwner();
	if (nullptr != MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotator;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotator);

		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		FVector TraceEndPoint = EyeLocation + (EyeRotator.Vector()*10000.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.bDebugQuery = true;
		QueryParams.bTraceComplex = false;

		FHitResult HitResult;
		if (true == GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEndPoint, ECollisionChannel::ECC_Visibility, QueryParams))
		{
			AActor* HitActor= HitResult.GetActor();
			
			UGameplayStatics::ApplyPointDamage(HitActor, DamageBase, EyeRotator.Vector(), HitResult, MyOwner->GetInstigatorController(), this, DamageType);
			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleEffect, HitResult.ImpactPoint,HitResult.ImpactNormal.Rotation(),true);
		}

		PlayFireEffect();

		if (static_cast<int32>(EDebugDrawType::DebugDrawType_Weapon) == DebugDrawType)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEndPoint, FColor::Red, false, 5.0f, 0, 1.0f);
		}

		
	}

}

void AWeaponRifle::PlayFireEffect()
{
	PlayMuzzleEffect();
}

void AWeaponRifle::PlayMuzzleEffect()
{
	if (nullptr != MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}
}

void AWeaponRifle::PlayTraceEffect()
{
}

void AWeaponRifle::BeginPlay()
{
	Super::BeginPlay();

}
