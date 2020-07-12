// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
// #include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		TEXT("Projectile Mesh")
	);
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("Projectile Movement")
	);
	// UProjectileMovementComponent don't have a transform, so they don't belong in the scene
	// hierarchy or inherit any transforms from their parent classes. 
	// Therefore, we don't need to do the usual process of attaching these to anything.

	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed = MovementSpeed;

	InitialLifeSpan = 3.f; // Inherited from actor, how long the actor lasts befor dying.
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}
