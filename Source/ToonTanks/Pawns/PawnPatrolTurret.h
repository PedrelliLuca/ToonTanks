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
	float PatrolSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol Movement", meta = (AllowPrivateAccess = "true"))
	float PatrolAmplitude = 300.f;

	FVector InitialLocation, CurrentLocation, TargetLocation;

	float Angle;

	bool bMoveTowardTarget = true;

	// FUNCTIONS
	FVector ComputeTargetLocation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	APawnPatrolTurret();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
