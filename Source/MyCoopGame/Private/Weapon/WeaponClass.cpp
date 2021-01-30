// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCoopGame/Public/Weapon/WeaponClass.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerCharacter.h"

AWeaponClass::AWeaponClass()
	:AItemBase()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetHiddenInGame(true);
}

void AWeaponClass::Use(APlayerCharacter * OwningCharacter)
{
	if (nullptr == OwningCharacter)
	{
		return;
	}

	OwningCharacter->EquipWeapon(this);
	ChangeState(EItemState::ItemState_Equip);
}

void AWeaponClass::Start()
{
}

void AWeaponClass::Stop()
{
}

EWeaponType AWeaponClass::GetWeaponType() const
{
	return WeaponType;
}

void AWeaponClass::OnChangeState()
{
	switch (ItemState)
	{
	case EItemState::ItemState_Field:
		SetHiddenPickupMesh(false);
		MeshComp->SetHiddenInGame(true);
		break;
	case EItemState::ItemState_Inventory:
		SetHiddenPickupMesh(true);
		MeshComp->SetHiddenInGame(true);
		break;
	case EItemState::ItemState_Equip:
		MeshComp->SetHiddenInGame(false);
		break;
	default:
		break;
	}
}
