// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnStalkerTurret.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ToonTanks/Pawns/PawnTank.h"

APawnStalkerTurret::APawnStalkerTurret()
{

}

void APawnStalkerTurret::BeginPlay()
{
    Super::BeginPlay();
}

void APawnStalkerTurret::Tick(float DeltaTime)
{
    // Handles TurretMesh movement
    // Super::Tick(DeltaTime);
    
    if (!PlayerPawn)
        return;

    // float DeltaMovement = StalkerSpeed * DeltaTime;
    RotateStalker(PlayerPawn->GetActorLocation());

}

void APawnStalkerTurret::RotateStalker(FVector LookAtTarget)
{
	// Update stalker rotation with respect to the world to face the target passed in
	FRotator StalkerRotation = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(),
		// We don't want the Z to change
		FVector(LookAtTarget.X, LookAtTarget.Y, GetActorLocation().Z)
	); 
	
	SetActorRotation(StalkerRotation);
}

// void APawnStalkerTurret::MoveStalkerTurret(float DeltaMovement)
// {
//     if (!PlayerPawn)
//         return;
    
//     FRotator StalkerRotation = UKismetMathLibrary::FindLookAtRotation(
// 		RootComponent->GetComponentLocation(),
// 		// We don't want the Z to change
// 		FVector(LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z)
// 	); 
	
// 	TurretMesh->SetWorldRotation(TurretRotation);
// }