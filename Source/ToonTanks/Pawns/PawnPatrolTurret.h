// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnTurret.h"
#include "PawnPatrolTurret.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API APawnPatrolTurret : public APawnTurret
{
	GENERATED_BODY()
	
private:
	// VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol Movement", meta = (AllowPrivateAccess = "true"))
	float PatrolSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol Movement", meta = (AllowPrivateAccess = "true"))
	float PatrolAmplitude = 500.f;

	FVector InitialLocation, CurrentLocation, TargetLocation;

	float Angle; // needed to move in the direction the RootComponent is facing
	float TotalMovement = 0.f; // How much of the PatrolAmplitude we traveled so far

	// true: we move from point A (Initial) to B (Target); false: we move from point B to A
	bool bMoveTowardTarget = true;

	// FUNCTIONS
	FVector ComputeTargetLocation();

	bool MovePatrolTurret(float DeltaMovement);

	void ReverseMovement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	APawnPatrolTurret();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
