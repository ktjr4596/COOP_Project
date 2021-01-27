// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponBase.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../MyCoopGame.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	// Set Roation using pawn's rotation
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	FRotator InitRoation(0.0f, -90.0f, 0.0f);
	FVector InitLocation(0.0f, 0.0f, -80.0f);
	GetMesh()->SetRelativeLocationAndRotation(InitLocation, InitRoation);
	
	// Set Character can crouch
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	bHasWeapon = false;

	// character movement followed by camera's direction
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	TargetItem = nullptr;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (nullptr != CameraComp)
	{
		DefaultFOV = CameraComp->FieldOfView;
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (nullptr != CameraComp)
	{
		float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
		float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomLerpSpeed);
		CameraComp->SetFieldOfView(NewFOV);
	}


	FVector EyeLocation;
	FRotator EyeRotator;
	GetActorEyesViewPoint(EyeLocation,EyeRotator);

	FVector TraceEndPoint = EyeLocation + (EyeRotator.Vector()*10000.0f);
	FVector BoxHalfSize(20.0f, 20.0f, 20.0f);

	TArray<AActor*> IgnoredActor;
	IgnoredActor.Add(this);
	FHitResult HitResult;
	if (static_cast<int32>(EDebugDrawType::DebugDrawType_EyeTrace) == DebugDrawType)
	{
		if (true == UKismetSystemLibrary::BoxTraceSingle(GetWorld(), EyeLocation, TraceEndPoint, BoxHalfSize, EyeRotator, ETraceTypeQuery::TraceTypeQuery1, false, IgnoredActor, EDrawDebugTrace::ForOneFrame, HitResult, true))
		{
			FVector PlayerLocation = GetActorLocation();
			FVector TargetLocation=HitResult.Actor->GetActorLocation();
			FVector DistanceToTarget = PlayerLocation - TargetLocation;
	
			// 블락된 아이템 텍스트 띄우기
			UKismetSystemLibrary::PrintString(GetWorld(), HitResult.Actor->GetName(), true, false, FLinearColor::Red, 0.0f);
			TargetItem = HitResult.Actor.Get();
			
		}
		else
		{
			TargetItem = nullptr;
		}
	}
	else
	{
		if (true == UKismetSystemLibrary::BoxTraceSingle(GetWorld(), EyeLocation, TraceEndPoint, BoxHalfSize, EyeRotator, ETraceTypeQuery::TraceTypeQuery1, false, IgnoredActor, EDrawDebugTrace::None, HitResult, true))
		{
			// 블락된 아이템 텍스트 띄우기
			FVector PlayerLocation = GetActorLocation();
			FVector TargetLocation = HitResult.Actor->GetActorLocation();
			FVector DistanceToTarget = PlayerLocation - TargetLocation;

			// 블락된 아이템 텍스트 띄우기
			TargetItem = HitResult.Actor.Get();

		}
		else
		{
			TargetItem = nullptr;
		}
	}


}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);

	// Bind Action
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &APlayerCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Released, this, &APlayerCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("EquipWeapon", EInputEvent::IE_Pressed, this, &APlayerCharacter::EquipWeapon);

	PlayerInputComponent->BindAction("Zoom", EInputEvent::IE_Pressed, this, &APlayerCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", EInputEvent::IE_Released, this, &APlayerCharacter::EndZoom);


	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &APlayerCharacter::UseWeapon);

	PlayerInputComponent->BindAction("Loot", EInputEvent::IE_Pressed, this, &APlayerCharacter::LootItem);
}

FVector APlayerCharacter::GetPawnViewLocation() const
{
	if (nullptr != CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void APlayerCharacter::MoveForward(float Value)
{
	FVector MoveDirection;
	if (true == bHasWeapon)
	{
		MoveDirection = GetActorForwardVector();
	}
	else
	{
		FRotator CamRotator = CameraComp->GetComponentRotation();
		FRotator Yaw(0.0f, CamRotator.Yaw, 0.0f);
		MoveDirection = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
	}
	AddMovementInput(MoveDirection*Value);
}

void APlayerCharacter::MoveRight(float Value)
{
	AddMovementInput(CameraComp->GetRightVector()*Value);
}

void APlayerCharacter::BeginCrouch()
{
	Crouch();
}

void APlayerCharacter::EndCrouch()
{
	UnCrouch();
}

void APlayerCharacter::EquipWeapon()
{
	if (true == bWasJumping || true==GetMovementComponent()->IsFalling())
	{
		return;
	}

	bHasWeapon = !bHasWeapon;
	if (true == bHasWeapon)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;

	}
	else
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	if (nullptr != Weapon)
	{
		Weapon->SetActorHiddenInGame(!bHasWeapon);
	}
}

void APlayerCharacter::BeginZoom()
{
	bWantsToZoom = true;
}

void APlayerCharacter::EndZoom()
{
	bWantsToZoom = false;
}

void APlayerCharacter::LootItem()
{
	if (nullptr != TargetItem)
	{
		if (nullptr != Weapon)
		{
			Weapon->Destroy();
			Weapon = nullptr;
		}
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AWeaponBase* NewWeapon= GetWorld()->SpawnActor<AWeaponBase>(TargetItem->GetClass(), GetActorTransform(), SpawnParams);
		if (nullptr != NewWeapon)
		{
			Weapon = NewWeapon;
			FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget,false);
		
			Weapon->AttachToComponent(GetMesh(), AttachRules, FName("WeaponSocket"));

			Weapon->SetActorEnableCollision(false);
		}
		
	}
}

void APlayerCharacter::UseWeapon()
{
	if (true==bHasWeapon&& nullptr != Weapon)
	{
		Weapon->Use();
	}
}

