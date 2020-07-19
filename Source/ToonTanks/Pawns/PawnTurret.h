// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnTurret.generated.h"

class APawnTank;

UCLASS()
class TOONTANKS_API APawnTurret : public APawnBase
{
	GENERATED_BODY()

public:

	APawnTurret();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called from TankGameModeBase.cpp
	int32 GetTurretScore() const;

protected:
	// FUNCTIONS
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void HandleDestruction() override;

	// VARIABLES
	// Stores reference to player.
	APawnTank* PlayerPawn = nullptr;

private:

	// Function which will be called on a repeating timer.
	void CheckFireCondition();

	// Function to check if the player's tank is in range.
	float ReturnDistanceToPlayer() const;

	// How often we call CheckFireCondition().
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", 
		meta = (AllowPrivateAccess = "true"))
	float FireRate = 2.f;

	// Turret's fire range.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", 
		meta = (AllowPrivateAccess = "true"))
	float FireRange = 500.f;

	// How many point is this kind of turret is worth to the player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scoring System", 
		meta = (AllowPrivateAccess = "true"))
	int32 TurretScore = 50;

	// Timer to bind and unbind CheckFireCondition() at runtime.
	FTimerHandle FireRateTimerHandle;

};
