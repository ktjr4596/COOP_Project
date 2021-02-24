// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "Weapon/WeaponClass.h"
#include "Items/ItemBase.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../MyCoopGame.h"
#include "MyCoopGame/Public/Items/InventoryComponent.h"
#include "Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/WeaponAmmo.h"
#include "Interactive/InteractiveActorBase.h"
#include "Blueprint/UserWidget.h"
#include "PlayerActionNames.h"



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

	NearItemInteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphere"));
	NearItemInteractSphere->SetupAttachment(GetMesh());

	NearItemInteractSphere->SetSphereRadius(500.0f);

	bHasWeapon = false;

	// character movement followed by camera's direction
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComp"));

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECollisionResponse::ECR_Ignore);
	

	InteractTraceRange = 200.0f;

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
	if (ROLE_Authority == GetLocalRole())
	{
		if (nullptr != HealthComp)
		{
			HealthComp->OnHealthChanged.AddDynamic(this, &APlayerCharacter::OnHealthChanged);
		}


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

	FVector TraceEndPoint = EyeLocation + (EyeRotator.Vector()*InteractTraceRange);
	FVector BoxHalfSize(20.0f, 20.0f, 20.0f);

	TArray<AActor*> IgnoredActor;
	IgnoredActor.Add(this);
	FHitResult HitResult;
	if (static_cast<int32>(EDebugDrawType::DebugDrawType_EyeTrace) == DebugDrawType)
	{
		if (true == UKismetSystemLibrary::SphereTraceSingle(GetWorld(), EyeLocation, TraceEndPoint, 20.0f, ETraceTypeQuery::TraceTypeQuery5, false, IgnoredActor, EDrawDebugTrace::ForOneFrame, HitResult, true))
		{
			UKismetSystemLibrary::PrintString(GetWorld(), HitResult.Actor->GetName(), true, false, FLinearColor::Red, 0.0f);
		}

		if (TargetItem != HitResult.Actor)
		{
			AActor* OldTarget = TargetItem.Get();
			TargetItem = HitResult.Actor;
			OnInteractActorChanged.Broadcast(TargetItem.Get(), OldTarget);
		}
	}
	else
	{
		if (true == UKismetSystemLibrary::SphereTraceSingle(GetWorld(), EyeLocation, TraceEndPoint, 20.0f, ETraceTypeQuery::TraceTypeQuery5, false, IgnoredActor, EDrawDebugTrace::None, HitResult, true))
		{

		}

		if (TargetItem != HitResult.Actor)
		{
			AActor* OldTarget = TargetItem.Get();
			TargetItem = HitResult.Actor;
			OnInteractActorChanged.Broadcast(TargetItem.Get(),OldTarget);
		}
	}


}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	const TMap<EPlayerActionName, FName>& ActionNames =  PlayerActionNames::GetActionNameMap();

	// Bind Axis
	 PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);

	// Bind Action
	PlayerInputComponent->BindAction(ActionNames[EPlayerActionName::ActionName_Crouch], EInputEvent::IE_Pressed, this, &APlayerCharacter::BeginCrouch);
	PlayerInputComponent->BindAction(ActionNames[EPlayerActionName::ActionName_Crouch], EInputEvent::IE_Released, this, &APlayerCharacter::EndCrouch);

	PlayerInputComponent->BindAction(ActionNames[EPlayerActionName::ActionName_Jump], EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(ActionNames[EPlayerActionName::ActionName_Zoom], EInputEvent::IE_Pressed, this, &APlayerCharacter::BeginZoom);
	PlayerInputComponent->BindAction(ActionNames[EPlayerActionName::ActionName_Zoom], EInputEvent::IE_Released, this, &APlayerCharacter::EndZoom);


	PlayerInputComponent->BindAction(ActionNames[EPlayerActionName::ActionName_Fire], EInputEvent::IE_Pressed, this, &APlayerCharacter::UseWeapon);
	PlayerInputComponent->BindAction(ActionNames[EPlayerActionName::ActionName_Fire], EInputEvent::IE_Released, this, &APlayerCharacter::UnUseWeapon);

	PlayerInputComponent->BindAction(ActionNames[EPlayerActionName::ActionName_Interact], EInputEvent::IE_Pressed, this, &APlayerCharacter::Interact);

	PlayerInputComponent->BindAction(ActionNames[EPlayerActionName::ActionName_ReloadAmmo], EInputEvent::IE_Pressed, this, &APlayerCharacter::Reload);

	PlayerInputComponent->BindAction(ActionNames[EPlayerActionName::ActionName_OpenInventory], EInputEvent::IE_Pressed, this, &APlayerCharacter::OpenInventory);
}

FVector APlayerCharacter::GetPawnViewLocation() const
{
	if (nullptr != CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

UInventoryComponent * APlayerCharacter::GetInventory()
{
	return InventoryComp;
}

bool APlayerCharacter::IsReloading()
{
	return bIsReloading;
}

void APlayerCharacter::OnHealthChanged(UHealthComponent * TargetHealthComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Health <= 0.0f && false == bIsDied)
	{

		UnUseWeapon();
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);

		bIsDied = true;
	}
}

TArray<AActor*> APlayerCharacter::CheckOverlappedActors(TSubclassOf<AActor> TargetClass)
{
	TArray<AActor*> OutActors;
	NearItemInteractSphere->GetOverlappingActors(OutActors, TargetClass);

	return OutActors;
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

	InventoryComp->RemoveItem(Item,EItemState::ItemState_Equip);

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

void APlayerCharacter::Interact()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerInteract();
	}
	if (nullptr != TargetItem)
	{
		AActor* CurrentTarget = TargetItem.Get();
	
		if (nullptr!=CurrentTarget && true == CurrentTarget->GetClass()->IsChildOf(AInteractiveActorBase::StaticClass()))
		{
			AInteractiveActorBase* CurrentTargetActor = Cast<AInteractiveActorBase>(CurrentTarget);
			if (nullptr != CurrentTargetActor)
			{
				CurrentTargetActor->Interact(this);
			}
		}
	}
}

void APlayerCharacter::ServerInteract_Implementation()
{
	Interact();
}


bool APlayerCharacter::ServerInteract_Validate()
{
	return true;
}

void APlayerCharacter::Reload()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerReload();
	}

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
					InventoryComp->RemoveItem(ResultItem[ii],EItemState::ItemState_None);
					bIsReloading = true;
					break;
				}
			}
		}
	}

}

void APlayerCharacter::ServerReload_Implementation()
{
	Reload();
}

bool APlayerCharacter::ServerReload_Validate()
{
	return true;
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
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerUseItem(Item);
	}

	if (nullptr == Item)
	{
		return;
	}

	Item->Use(this);
	Item->OnUse(this);
}

void APlayerCharacter::ServerUseItem_Implementation(AItemBase* Item)
{
	UseItem(Item);
}

bool APlayerCharacter::ServerUseItem_Validate(AItemBase* Item)
{
	return true;
}

void APlayerCharacter::OpenInventory()
{
	AController* MyController = GetController();
	if (nullptr != MyController)
	{
		APlayerController* PlayerController = Cast<APlayerController>(MyController);

		if (nullptr != InventoryWidget)
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			InventoryWidget = CreateWidget(PlayerController, InventoryWidgetClass);
			InventoryWidget->AddToViewport();
		}

		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;

		InventoryWidget->SetFocus();

		UInputSettings* InputSetting = UInputSettings::GetInputSettings();
		TArray<FInputActionKeyMapping> KeyMappingResult;

		FName ActionName = PlayerActionNames::GetActionName(EPlayerActionName::ActionName_OpenInventory);

		InputSetting->GetActionMappingByName(ActionName, KeyMappingResult);

		if (0 < KeyMappingResult.Num())
		{
			OnActionDetected.Broadcast(KeyMappingResult[0].Key);
		}

	}
}

FGenericTeamId APlayerCharacter::GetGenericTeamId() const
{
	return TeamID;
}



// Relicated로 선언된 변수들을 엔진 쪽에서 인식할 수 있는 방법
void APlayerCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, Weapon);
	DOREPLIFETIME(APlayerCharacter, bHasWeapon);
	DOREPLIFETIME(APlayerCharacter, TargetItem);
	DOREPLIFETIME(APlayerCharacter, bIsDied);
	DOREPLIFETIME(APlayerCharacter, bIsReloading);
}