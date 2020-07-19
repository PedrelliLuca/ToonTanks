// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnTurret.h"
#include "PawnStalkerTurret.generated.h"

UCLASS()
class TOONTANKS_API APawnStalkerTurret : public APawnTurret
{
	GENERATED_BODY()
	
private:
	// VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stalker Movement", meta = (AllowPrivateAccess = "true"))
	float StalkerSpeed = 100.f;

	FVector CurrentLocation;

	// FUNCTIONS
	// void MoveStalkerTurret(float DeltaMovement);

	void RotateStalker(FVector LookAtTarget);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	APawnStalkerTurret();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
