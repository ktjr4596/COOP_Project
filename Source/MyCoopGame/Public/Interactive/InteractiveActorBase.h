// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveActorBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractActor);


class APlayerCharacter;

UCLASS(Abstract, BlueprintType,Blueprintable)
class MYCOOPGAME_API AInteractiveActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveActorBase();

public:

	UFUNCTION(BlueprintNativeEvent, Category="Interact")
	void Interact(APlayerCharacter* Character);

	virtual void Interact_Implementation(APlayerCharacter* Charcater);

protected:
	/** Broadcast when interact this actor */
	FOnInteractActor OnInteract;

	/** Actor's name for display  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interact")
	FText ActorName;

	/** Action name to find action mapping for display */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Action")
	FText ActionName;


};
