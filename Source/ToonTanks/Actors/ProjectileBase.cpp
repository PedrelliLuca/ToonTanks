// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		TEXT("Projectile Mesh")
	);
	// Binding delegate function to OnComponentHit
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
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

// Thanks to dynamic binding, this is automatically called everytime the StaticMesh OnComponentHit
// function is called (i.e. everytime the mesh hits (or is hit by) something solid.)
void AProjectileBase::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	// We get a reference to the owning actor, i.e. the pawn that spawns our projectile
	AActor* MyOwner = GetOwner();

	// If for some reason this we can't get a valid ref, return as we need to check against the
	// owner
	if (!MyOwner)
		return;

	// We deal damage only if the following conditions on the other actor are all true:
	if (OtherActor != NULL && OtherActor != this && OtherActor != MyOwner)
	{
		// We hit a valid actor!! ApplyDamage calls OnTakeAnyDamage on OtherActor.
		UGameplayStatics::ApplyDamage(
			OtherActor,
			Damage,
			MyOwner->GetInstigatorController(), // Reference to owning player controller
			this, // Damage causer
			DamageType
		);
	}

	// TODO: some visual effects 

	// We finally remove the projectile from play
	Destroy();
}