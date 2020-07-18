// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPack.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "ToonTanks/Pawns/PawnTank.h"


// Sets default values
AHealthPack::AHealthPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthPackMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		TEXT("HealthPack Mesh")
	);
	// Binding delegate function to OnComponentHit
	HealthPackMesh->OnComponentHit.AddDynamic(this, &AHealthPack::OnHit);
	RootComponent = HealthPackMesh;
}

// Called when the game starts or when spawned
void AHealthPack::BeginPlay()
{
	Super::BeginPlay();

	PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void AHealthPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// A simple animation
	SetActorRotation(
		GetActorRotation() + FRotator(0.f, AngularVelocity * DeltaTime, 0.f)
	);
}

// Thanks to dynamic binding, this is automatically called everytime the StaticMesh OnComponentHit
// function is called (i.e. everytime the mesh hits (or is hit by) something solid.)
void AHealthPack::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	if (!PlayerTank)
		return;

	// We heal only if the other actor is the player pawn
	if (OtherActor != nullptr && PlayerTank == OtherActor)
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,
			-Healing, // A negative value adds health instead of removing it
			GetInstigatorController(), // Reference to owning player controller
			this, // Damage causer
			DamageType
		);

		// Visual effect
		if (HitParticle)
			UGameplayStatics::SpawnEmitterAtLocation(
				this,
				HitParticle,
				GetActorLocation(),
				FRotator::ZeroRotator
			);
		
		// Sound effect
		if (HitSound)
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		
		// We finally remove the health pack from play
		Destroy();
	}
}