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

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(TEXT("COOP.DebugWeapons"), DebugWeaponDrawing, TEXT("Draw Debug Lines for weapons"), ECVF_Cheat);

AWeaponRifle::AWeaponRifle()
	:AWeaponClass()
{
}

void AWeaponRifle::Use(APlayerCharacter* OwningCharacter)
{
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds,0.0f);

	GetWorldTimerManager().SetTimer(TimerHandle_Firing, this, &AWeaponRifle::Fire, TimeBetweenShots, true, FirstDelay);
}

//void AWeaponRifle::UnUse()
//{
//	StopFire();
//}

void AWeaponRifle::Fire()
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
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.bDebugQuery = true;
		QueryParams.bTraceComplex = false;
		QueryParams.bReturnPhysicalMaterial = true;

		FHitResult HitResult;
		if (true == GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEndPoint, COLLISION_WEAPON, QueryParams))
		{
			AActor* HitActor = HitResult.GetActor();


			EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());


			float ActualDamage = DamageBase;

			if (SURFACETYPE_FLESHVLUNERABLE == HitSurfaceType)
			{
				ActualDamage *= 4.0f;
			}
			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, EyeRotator.Vector(), HitResult, MyOwner->GetInstigatorController(), this, DamageType);


			PlayImpactEffect(HitSurfaceType, HitResult.ImpactPoint, HitResult.ImpactNormal);


		}

		PlayFireEffect();

		if (static_cast<int32>(EDebugDrawType::DebugDrawType_Weapon) == DebugDrawType)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEndPoint, FColor::Red, false, 5.0f, 0, 1.0f);
		}

		LastFireTime = GetWorld()->TimeSeconds;

	}
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


void AWeaponRifle::PlayImpactEffect(EPhysicalSurface SurfaceType, const FVector & ImpactPoint, const FVector & ImpactNormal)
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
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, ImpactPoint, ImpactNormal.Rotation(), true);
	}
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

	TimeBetweenShots = 60.0f / RateOfFire;

}
