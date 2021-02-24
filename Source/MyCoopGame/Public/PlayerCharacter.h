// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIModule/Classes/GenericTeamAgentInterface.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponChanged,class APlayerCharacter*, Character,  class AWeaponClass*, ChangedWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractActorChanged, class AActor*, TargetActor, class AActor* , OldActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNearItemDetected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionDetected, struct FKey, KeyType);

class UCameraComponent;
class USpringArmComponent;
class AItemBase;
class AWeaponClass;
class UInventoryComponent;
class UHealthComponent;
class USphereComponent;
class UUserWidget;




UCLASS()
class MYCOOPGAME_API APlayerCharacter : public ACharacter ,public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;
	
	UInventoryComponent* GetInventory();

	bool IsReloading();

public: 
	void EquipWeapon(AItemBase* Item);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEquipWeapon(AItemBase* Item);

	// Call if player health is changed
	UFUNCTION()
	void OnHealthChanged(class UHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category="Interact")
	TArray<AActor*> CheckOverlappedActors(TSubclassOf<AActor> TargetClass);

protected:
	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	void BeginZoom();

	void EndZoom();

	void Interact();

	UFUNCTION(Server, WithValidation, Reliable)
	void ServerInteract();

	void Reload();

	UFUNCTION(Server,WithValidation,Reliable)
	void ServerReload();

	UFUNCTION(BlueprintCallable,Category="Weapon")
	void UseWeapon();

	void UnUseWeapon();

	UFUNCTION(BlueprintCallable, Category="Item")
	void UseItem(AItemBase* Item);

	UFUNCTION(Server, WithValidation, Reliable)
	void ServerUseItem(AItemBase* Item);

	void OpenInventory();

private:
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	UPROPERTY(BlueprintAssignable, Category = "WeaponEvents")
	FOnWeaponChanged OnWeaponChange;

	UPROPERTY(BlueprintAssignable, Category="InteractEvents")
	FOnInteractActorChanged OnInteractActorChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable , Category = "InteractEvents")
	FOnNearItemDetected OnNearItemDetected;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="ActionEvents")
	FOnActionDetected OnActionDetected;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Team")
	FGenericTeamId TeamID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category="Weapon")
	AWeaponClass* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  Category="Inventory")
	UInventoryComponent* InventoryComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Health")
	UHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interact")
	USphereComponent* NearItemInteractSphere;

	TWeakObjectPtr<UUserWidget> InventoryWidget;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Inventory")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY(Replicated)
	TWeakObjectPtr<AActor> TargetItem;

	float DefaultFOV;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Zoom")
	float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly , Category="Zoom", meta=(ClampMin=0.1 , ClampMax=100.0))
	float ZoomLerpSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interact", meta=(ClampMin=1.0))
	float InteractTraceRange;

	UPROPERTY(BlueprintReadOnly, Category="Zoom")
	bool bWantsToZoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Replicated,  Category="Weapon")
	bool bHasWeapon;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Camera")
	bool bCameraRotating;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category="Player")
	bool bIsDied;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="Weapon")
	bool bIsReloading;
};
