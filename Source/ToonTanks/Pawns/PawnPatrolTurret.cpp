// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnPatrolTurret.h"
#include "Kismet/KismetMathLibrary.h"

APawnPatrolTurret::APawnPatrolTurret()
{

}

void APawnPatrolTurret::BeginPlay()
{
    Super::BeginPlay();

    InitialLocation = GetActorLocation();
    Angle = GetActorRotation().Yaw * UKismetMathLibrary::GetPI() / 180;
    UE_LOG(LogTemp, Warning, TEXT("Angle: %f"), Angle);

    TargetLocation = ComputeTargetLocation();
    UE_LOG(LogTemp, Warning, TEXT("InitialLocation: %s"), *InitialLocation.ToString());
    UE_LOG(LogTemp, Warning, TEXT("TargetLocation: %s"), *TargetLocation.ToString());
}

// Function to compute the other extreme of the patrol, called only at BeginPlay
FVector APawnPatrolTurret::ComputeTargetLocation()
{
    float AmplitudeX, AmplitudeY;

    // The angle must be in radians for Cos/Sin to work as expected.
    AmplitudeX = PatrolAmplitude * FGenericPlatformMath::Cos(Angle);
    AmplitudeY = PatrolAmplitude * FGenericPlatformMath::Sin(Angle);
    UE_LOG(LogTemp, Warning, TEXT("AmplitudeX: %f"), AmplitudeX);
    UE_LOG(LogTemp, Warning, TEXT("AmplitudeY: %f"), AmplitudeY);

    return FVector(
        InitialLocation.X + AmplitudeX,
        InitialLocation.Y + AmplitudeY,
        InitialLocation.Z // No movement along Z direction, obviously
    );
}

// Called every frame
void APawnPatrolTurret::Tick(float DeltaTime)
{
    // Handles TurretMesh movement
    Super::Tick(DeltaTime);
    
    CurrentLocation = GetActorLocation();

    if (bMoveTowardTarget)
    {
        UE_LOG(LogTemp, Warning, TEXT("Towards Target"));
        SetActorLocation(UKismetMathLibrary::VInterpTo(
            CurrentLocation,
            TargetLocation,
            DeltaTime,
            PatrolSpeed
        ));

        if (CurrentLocation == TargetLocation)
            bMoveTowardTarget = false;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Towards Initial"));
        SetActorLocation(UKismetMathLibrary::VInterpTo(
            CurrentLocation,
            InitialLocation,
            DeltaTime,
            PatrolSpeed
        ));

        if (CurrentLocation == InitialLocation)
            bMoveTowardTarget = true;
    }
}