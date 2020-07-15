// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnBase.generated.h"

// You need this list of classes when something you're using in the header is not already in the
// includes above. The actual includes will be in the related .cpp
class UCapsuleComponent;
class AProjectileBase;
class UHealthComponent;


UCLASS()
class TOONTANKS_API APawnBase : public APawn
{
	GENERATED_BODY()

private:
	// All components go in this section

	// The capsule components will act as root component and the general collision for the pawns
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComp = nullptr;

	// One mesh component is the base mesh, one is the turret mesh. This will allow them to rotate independently
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh = nullptr;

	// Scene component to act as the projectile spawn point7
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent = nullptr;

	// Variables.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* DeathParticle;

public:
	// Sets default values for this pawn's properties
	APawnBase();

	void PawnDestroyed(); 

protected:
	// For PawnTank: the FVector is the mouse location
	// For PawnTurret: the FVector is the PlayerTank location
	void RotateTurret(FVector LookAtTarget);

	// Function to spawn a projectile in the direction the turret is facing
	void Fire();

	// Function to control how the pawn reacts when destroyed
	virtual void HandleDestruction();
};
