// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponAmmo.h"


AWeaponAmmo::AWeaponAmmo()
{

}

EAmmoType AWeaponAmmo::GetAmmoType() const
{
	return AmmoType;
}

void AWeaponAmmo::OnChangeState()
{
	switch (ItemState)
	{
	case EItemState::ItemState_Field:
		SetHiddenPickupMesh(false);
		break;
	case EItemState::ItemState_Inventory:
		SetHiddenPickupMesh(true);
		break;
	case EItemState::ItemState_Equip:
		break;
	default:
		break;
	}
}