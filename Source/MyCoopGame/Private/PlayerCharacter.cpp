// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/WeaponClass.h"
#include "Items/ItemBase.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../MyCoopGame.h"
#include "MyCoopGame/Public/Items/InventoryComponent.h"
#include "Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/WeaponAmmo.h"

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

	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComp"));

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECollisionResponse::ECR_Ignore);

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


	// Bind onhealthchange function to UHealthComponent
	if (nullptr != HealthComp)
	{
		HealthComp->OnHealthChanged.AddDynamic(this, &APlayerCharacter::OnHealthChanged);
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
			TargetItem = HitResult.Actor;
			
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
			TargetItem = HitResult.Actor;

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

	PlayerInputComponent->BindAction("Zoom", EInputEvent::IE_Pressed, this, &APlayerCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", EInputEvent::IE_Released, this, &APlayerCharacter::EndZoom);


	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &APlayerCharacter::UseWeapon);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &APlayerCharacter::UnUseWeapon);

	PlayerInputComponent->BindAction("Loot", EInputEvent::IE_Pressed, this, &APlayerCharacter::LootItem);

	PlayerInputComponent->BindAction("ReloadAmmo", EInputEvent::IE_Pressed, this, &APlayerCharacter::Reload);
}

FVector APlayerCharacter::GetPawnViewLocation() const
{
	if (nullptr != CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void APlayerCharacter::OnHealthChanged(UHealthComponent * TargetHealthComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Health <= 0.0f && false == bIsDied)
	{
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);

		bIsDied = true;
	}
}

void APlayerCharacter::MoveForward(float Value)
{
	FVector MoveDirection;
	if (true == bHasWeapon || true==bCameraRotating)
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
	FVector MoveDirection;
	if (true == bCameraRotating)
	{
		MoveDirection = GetActorRightVector();
	}
	else
	{
		MoveDirection=CameraComp->GetRightVector();
	}
	AddMovementInput(MoveDirection*Value);
}

void APlayerCharacter::BeginCrouch()
{
	Crouch();
}

void APlayerCharacter::EndCrouch()
{
	UnCrouch();
}

void APlayerCharacter::EquipWeapon(AItemBase* Item)
{

	if (GetLocalRole() < ROLE_Authority)
	{
		ServerEquipWeapon(Item);
	}
	if (nullptr == Item)
	{
		return;
	}


	EItemType ItemType = Item->GetItemType();

	if (EItemType::ItemType_Equipable != ItemType)
	{
		return;
	}

	AWeaponClass* TargetWeapon = Cast<AWeaponClass>(Item);
	if (nullptr != TargetWeapon)
	{
		if (nullptr != Weapon)
		{
			Weapon->SetOwner(nullptr);
			InventoryComp->AddItem(Weapon);
		}

		Weapon = TargetWeapon;
		Weapon->SetOwner(this);

		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, false);

		Weapon->AttachToComponent(GetMesh(), AttachRules, FName("WeaponSocket"));

		Weapon->SetActorEnableCollision(false);
		UKismetSystemLibrary::PrintString(GetWorld(), Weapon->GetName(), true, true, FLinearColor::Green, 5.0f);
	}

	bHasWeapon = true;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	InventoryComp->RemoveItem(Item);
	Weapon->ChangeState(EItemState::ItemState_Equip);

	OnWeaponChange.Broadcast(this,Weapon);
}


void APlayerCharacter::ServerEquipWeapon_Implementation(AItemBase* Item)
{
	EquipWeapon(Item);
}

bool APlayerCharacter::ServerEquipWeapon_Validate(AItemBase* Item)
{
	return true;
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
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerLootItem();
	}
	if (nullptr != TargetItem)
	{
		AActor* CurrentTarget = TargetItem.Get();
		if (true == CurrentTarget->GetClass()->IsChildOf(AItemBase::StaticClass()))
		{

			AItemBase* CurrentItem = Cast<AItemBase>(CurrentTarget);
			if (nullptr == CurrentItem)
			{
				return;
			}
			InventoryComp->AddItem(CurrentItem);


		}
		
	}
}

void APlayerCharacter::ServerLootItem_Implementation()
{
	LootItem();
}


bool APlayerCharacter::ServerLootItem_Validate()
{
	return true;
}

void APlayerCharacter::Reload()
{
	if (nullptr == Weapon || nullptr==InventoryComp)
	{
		return;
	}

	TArray<AItemBase*> ResultItem= InventoryComp->GetItemsByType(EItemType::ItemType_Ammo);

	if (0< ResultItem.Num())
	{
		int32 ItemCount = ResultItem.Num();
		EAmmoType CurrentAmmoType= Weapon->GetAmmoType();
		for (int32 ii = 0; ii < ItemCount; ++ii)
		{
			AWeaponAmmo* TargetAmmo= 	Cast<AWeaponAmmo>(ResultItem[ii]);
			if (nullptr != TargetAmmo)
			{
				if (CurrentAmmoType == TargetAmmo->GetAmmoType())
				{
					Weapon->ResetAmmo();
					InventoryComp->RemoveItem(ResultItem[ii]);
					break;
				}
			}
		}
	}

}

void APlayerCharacter::UseWeapon()
{
	if (true==bHasWeapon&& nullptr != Weapon)
	{
		Weapon->Start();
	}

}

void APlayerCharacter::UnUseWeapon()
{
	if (true == bHasWeapon && nullptr != Weapon)
	{
		Weapon->Stop();
	}
}

void APlayerCharacter::UseItem(AItemBase * Item)
{
	if (nullptr == Item)
	{
		return;
	}

	Item->Use(this);
	Item->OnUse(this);
}



// Relicated로 선언된 변수들을 엔진 쪽에서 인식할 수 있는 방법
void APlayerCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, Weapon);
	DOREPLIFETIME(APlayerCharacter, bHasWeapon);
	DOREPLIFETIME(APlayerCharacter, TargetItem);
	DOREPLIFETIME(APlayerCharacter, bIsDied);
}