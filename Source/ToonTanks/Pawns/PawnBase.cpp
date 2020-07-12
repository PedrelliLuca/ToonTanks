// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnBase.h"
#include "Components/CapsuleComponent.h"

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
	// Update turret rotation with respect to the base to face the target passed in by child class
	// TurretMesh->SetWorldRotation()...
}

void APawnBase::Fire()
{
	// Get ProjectileSpawnPoint Location && Rotation
	// Spawn a projectile instance at location towards rotation
}

void APawnBase::HandleDestruction()
{
	// Universal functionality ---
	// Play death effect particles, sound and camera shake.

	// Then do unique child overrides
	// -- PawnTurret - Call GameMode to inform it the turret died -> then Destroy() self.

	// -- PawnTank - Inform GameMode player died -> Hide() all components && stop movement Input.
}
