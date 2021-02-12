// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponRifle.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "../MyCoopGame.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(TEXT("COOP.DebugWeapons"), DebugWeaponDrawing, TEXT("Draw Debug Lines for weapons"), ECVF_Cheat);

AWeaponRifle::AWeaponRifle()
	:AWeaponClass()
{

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 32.0f;

}

void AWeaponRifle::Start()
{
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
	GetWorldTimerManager().SetTimer(TimerHandle_Firing, this, &AWeaponRifle::Fire, TimeBetweenShots, true, FirstDelay);
}

void AWeaponRifle::Stop()
{
	StopFire();
}

void AWeaponRifle::ResetAmmo()
{
	CurrentAmmo = DefaultAmmo;
	OnAmmoChange.Broadcast(CurrentAmmo);
}

void AWeaponRifle::Fire()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerFire();
	}

	if (CurrentAmmo <= 0)
	{
		return;
	}

	AActor* MyOwner = GetOwner();
	if (nullptr != MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotator;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotator);

		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		FVector TraceEndPoint = EyeLocation + (EyeRotator.Vector()*10000.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.bDebugQuery = true;
		QueryParams.bTraceComplex = false;
		QueryParams.bReturnPhysicalMaterial = true;

		EPhysicalSurface HitSurfaceType = SurfaceType_Default;
		FHitResult HitResult;

		if (true == GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEndPoint, COLLISION_WEAPON, QueryParams))
		{
			AActor* HitActor = HitResult.GetActor();

			HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());

			float ActualDamage = DamageBase;

			if (SURFACETYPE_FLESHVLUNERABLE == HitSurfaceType)
			{
				ActualDamage *= 4.0f;
			}
			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, EyeRotator.Vector(), HitResult, MyOwner->GetInstigatorController(), MyOwner, DamageType);


			PlayImpactEffect(HitSurfaceType, HitResult.ImpactPoint);

			TraceEndPoint = HitResult.ImpactPoint;
		}

		PlayFireEffect();

		if (ROLE_Authority == GetLocalRole())
		{
			HitScanTrace.HitSurfaceType = HitSurfaceType;
			HitScanTrace.HitImpactPoint = TraceEndPoint;
			++HitScanTrace.Seed;
		}

		if (static_cast<int32>(EDebugDrawType::DebugDrawType_Weapon) == DebugDrawType)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEndPoint, FColor::Red, false, 5.0f, 0, 1.0f);
		}
		--CurrentAmmo;
		
		LastFireTime = GetWorld()->TimeSeconds;
		OnAmmoChange.Broadcast(CurrentAmmo);

	}
}

void AWeaponRifle::ServerFire_Implementation()
{
	Fire();
}

bool AWeaponRifle::ServerFire_Validate()
{
	return true;
}

void AWeaponRifle::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Firing);
}

void AWeaponRifle::PlayFireEffect()
{
	PlayMuzzleEffect();

	APawn* MyOwner = Cast<APawn>( GetOwner());

	if (nullptr != MyOwner)
	{
		APlayerController* PlayerController= Cast<APlayerController>( MyOwner->GetController());
		if (nullptr != PlayerController)
		{
			PlayerController->ClientPlayCameraShake(FireCamShake);
		}
	}

}


void AWeaponRifle::PlayImpactEffect(EPhysicalSurface SurfaceType, const FVector & ImpactPoint)
{
	UParticleSystem* ImpactEffect = nullptr;
	switch (SurfaceType)
	{
	case SURFACETYPE_FLESHDEFAULT:
	case SURFACETYPE_FLESHVLUNERABLE:
		ImpactEffect = FleshEffect;
		break;
	default:
		ImpactEffect = MuzzleEffect;
		break;
	}
	if (nullptr != ImpactEffect)
	{

		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, ImpactPoint, ShotDirection.Rotation());
	}
}

void AWeaponRifle::PlayMuzzleEffect()
{
	if (nullptr != MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}
}


void AWeaponRifle::OnRep_HitScanTrace()
{
	PlayMuzzleEffect();
	PlayImpactEffect(HitScanTrace.HitSurfaceType, HitScanTrace.HitImpactPoint);
}

void AWeaponRifle::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60.0f / RateOfFire;

	CurrentAmmo = DefaultAmmo;

}


// Relicated로 선언된 변수들을 엔진 쪽에서 인식할 수 있는 방법
void AWeaponRifle::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AWeaponRifle, HitScanTrace, COND_SkipOwner);
}