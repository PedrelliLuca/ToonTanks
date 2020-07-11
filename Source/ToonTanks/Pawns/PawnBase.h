// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnBase.generated.h"

// This component is not already in the includes above
class UCapsuleComponent;

UCLASS()
class TOONTANKS_API APawnBase : public APawn
{
	GENERATED_BODY()

private:
	// All components go in this section

	// The capsule components will act as root component and the general collision for the pawns
	UPROPERTY()
	UCapsuleComponent* CapsuleComp;

	// One mesh component is the base mesh, one is the turret mesh. This will allow them to rotate independently
	UPROPERTY()
	UStaticMeshComponent* BaseMesh;
	UPROPERTY()
	UStaticMeshComponent* TurretMesh;


	// Scene component to act as the projectile spawn point
	USceneComponent* ProjectileSpawnPoint;

public:
	// Sets default values for this pawn's properties
	APawnBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
