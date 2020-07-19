// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnPatrolTurret.h"
#include "Kismet/KismetMathLibrary.h"

APawnPatrolTurret::APawnPatrolTurret()
{

}

void APawnPatrolTurret::BeginPlay()
{
    Super::BeginPlay();

    // The angle must be in radians for the Cos/Sin functions to work.
    Angle = GetActorRotation().Yaw * UKismetMathLibrary::GetPI() / 180;
}

// Called every frame
void APawnPatrolTurret::Tick(float DeltaTime)
{
    // Handles TurretMesh movement
    Super::Tick(DeltaTime);

    if (!bActive) // Patrolling turrets do not move if inactive
        return;

    TotalMovement += PatrolSpeed * DeltaTime;
    float DeltaMovement = PatrolSpeed * DeltaTime;

    if (TotalMovement > PatrolAmplitude) // Did we reach the patrol limit?
    {
        // We move less to avoid going beyond the patrol limit
        DeltaMovement -= TotalMovement - PatrolAmplitude;

        // We don't care of collisions since we reverse movement anyways in this case
        MovePatrolTurret(DeltaMovement); 
        ReverseMovement();
    }
    else
    {
        bool bNoCollisions = MovePatrolTurret(DeltaMovement);
        if (!bNoCollisions) // Did we collide?
            ReverseMovement();
    }
}

// Returns true if no collision happend and the movement was successfull
bool APawnPatrolTurret::MovePatrolTurret(float DeltaMovement)
{
    int32 Direction = bMoveTowardTarget ? +1 : -1;
    CurrentLocation = GetActorLocation();

    bool bNoCollision = SetActorLocation(
        FVector(
            CurrentLocation.X + Direction * DeltaMovement * FGenericPlatformMath::Cos(Angle),
            CurrentLocation.Y + Direction * DeltaMovement * FGenericPlatformMath::Sin(Angle),
            CurrentLocation.Z
        ),
        true // Sweep enabled to check for collisions
    );

    return bNoCollision;
}

// We reverse when either a collision occurs or we reached the patrol limit.
void APawnPatrolTurret::ReverseMovement()
{
    bMoveTowardTarget = !bMoveTowardTarget;
    TotalMovement = 0.f;
}