// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponRifle.h"
#include "Engine/World.h"
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
		FVector EyeLocation;
		FRotator EyeRotator;
		MyOWner->GetActorEyesViewPoint(EyeLocation, EyeRotator);


		FHitResult HitResult;
		FVector TraceEndPoint = EyeLocation + (EyeRotator.Vector()*10000.0f);
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.bDebugQuery = true;
		QueryParams.bTraceComplex = false;
		if (true == GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEndPoint, ECollisionChannel::ECC_Visibility, QueryParams))
		{

		}

		DrawDebugLine(GetWorld(), EyeLocation, TraceEndPoint, FColor::Red, false, 0.0f);


	}

}

void AWeaponRifle::BeginPlay()
{
	Super::BeginPlay();

}
