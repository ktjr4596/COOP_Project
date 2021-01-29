// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCoopGame/Public/Weapon/WeaponClass.h"

AWeaponClass::AWeaponClass()
{

}

EWeaponType AWeaponClass::GetWeaponType() const
{
	return WeaponType;
}
