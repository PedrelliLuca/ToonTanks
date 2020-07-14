// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ToonTanks/Actors/ProjectileBase.h"

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	RootComponent = CapsuleComp;

	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	// We want the mesh to inherit the movement and the rotation from the capsule component
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);  // movement & rotation inherited from base mesh

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh); // movement & rotation inherited from turret mesh
}

void APawnBase::RotateTurret(FVector LookAtTarget)
{
	// Update turret rotation with respect to the world to face the target passed in by child class

	FRotator TurretRotation = UKismetMathLibrary::FindLookAtRotation(
		TurretMesh->GetComponentLocation(),
		// We don't want the Z to change
		FVector(LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z)
	); 
	
	TurretMesh->SetWorldRotation(TurretRotation);
}

void APawnBase::Fire()
{
	if (ProjectileClass && ProjectileSpawnPoint)
	{
		// Spawn a projectile instance at location towards rotation. "Temp" because it lives for 3s.
		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(
			ProjectileClass,
			ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation()
		);
		TempProjectile->SetOwner(this); // Needed in AProjectileBase::OnHit to avoid self-damage
	}
}

void APawnBase::HandleDestruction()
{
	// Universal functionality ---
	// Play death effect particles, sound and camera shake.

	// Then do unique child overrides
	// -- PawnTurret - Call GameMode to inform it the turret died -> then Destroy() self.

	// -- PawnTank - Inform GameMode player died -> Hide() all components && stop movement Input.
}

void APawnBase::PawnDestroyed()
{
	HandleDestruction();
}