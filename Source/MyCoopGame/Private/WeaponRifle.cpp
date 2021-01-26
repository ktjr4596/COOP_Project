// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponRifle.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"

AWeaponRifle::AWeaponRifle()
	:AWeaponBase()
{

}



void AWeaponRifle::Use()
{
	AActor* MyOWner = GetOwner();
	if (nullptr != MyOWner)
	{

		FTransform MuzzleTransform = MeshComp->GetSocketTransform(MuzzleSocketName,RTS_World);

		FVector TraceEndPoint = MuzzleTransform.GetLocation() + (MuzzleTransform.GetRotation().GetUpVector()*10000.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.bDebugQuery = true;
		QueryParams.bTraceComplex = false;

		FHitResult HitResult;
		if (true == GetWorld()->LineTraceSingleByChannel(HitResult, MuzzleTransform.GetLocation(), TraceEndPoint, ECollisionChannel::ECC_Visibility, QueryParams))
		{

		}

		DrawDebugLine(GetWorld(), MuzzleTransform.GetLocation(), TraceEndPoint, FColor::Red, false, 5.0f,0,1.0f);


	}

}

void AWeaponRifle::BeginPlay()
{
	Super::BeginPlay();

}
