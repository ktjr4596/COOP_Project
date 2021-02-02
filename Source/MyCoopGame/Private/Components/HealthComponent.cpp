// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "..\..\Public\Components\HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	DefaultHealth = 100.0f;
	SetIsReplicated(true);
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (GetOwnerRole() == ROLE_Authority)
	{
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
	}

	CurrentHealth = DefaultHealth;

}

void UHealthComponent::HandleTakeAnyDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}

	CurrentHealth= FMath::Clamp((CurrentHealth - Damage), 0.0f, DefaultHealth);

	OnHealthChanged.Broadcast(this,CurrentHealth,Damage,DamageType,InstigatedBy,DamageCauser);
}

void UHealthComponent::OnRep_HealthChanged(float OldHealth)
{
	float DeltaHealth = CurrentHealth - OldHealth;

	OnHealthChanged.Broadcast(this,CurrentHealth,DeltaHealth, nullptr,nullptr,nullptr);
}


// Relicated로 선언된 변수들을 엔진 쪽에서 인식할 수 있는 방법
void UHealthComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}