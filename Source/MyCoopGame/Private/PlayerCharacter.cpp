// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponBase.h"

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

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
}

