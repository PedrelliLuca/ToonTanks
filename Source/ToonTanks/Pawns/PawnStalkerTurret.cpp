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
    // This would move the head, but it's useless since the whole pawn has to move for stalkers.
    // Super::Tick(DeltaTime);
    
    if (!PlayerPawn)
        return;

    PlayerLocation = PlayerPawn->GetActorLocation();
    StalkerLocation = GetActorLocation();
    RotateStalker();
    MoveStalker(StalkerSpeed * DeltaTime);
}

void APawnStalkerTurret::RotateStalker()
{
	// Update stalker rotation with respect to the world to face the target passed in
	FRotator StalkerRotation = UKismetMathLibrary::FindLookAtRotation(
		StalkerLocation,
		FVector(PlayerLocation.X, PlayerLocation.Y, StalkerLocation.Z) // We don't want the Z to change
	); 
	
	SetActorRotation(StalkerRotation);
}

void APawnStalkerTurret::MoveStalker(float DeltaMovement)
{
    // Find the unit vector from the stalker to the player
    FVector MovementDirection = UKismetMathLibrary::GetDirectionUnitVector(
        StalkerLocation,
        PlayerLocation
    );

    // Move on that direction of DeltaMovement, with sweep active
    SetActorLocation(
        FVector(
            StalkerLocation.X + MovementDirection.X * DeltaMovement,
            StalkerLocation.Y + MovementDirection.Y * DeltaMovement,
            StalkerLocation.Z
        ), 
        true
    );
}