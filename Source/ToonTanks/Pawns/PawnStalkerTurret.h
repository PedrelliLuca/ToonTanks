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

	bool bMove = false;

	FVector StalkerLocation, PlayerLocation;

	// FUNCTIONS
	void MoveStalker(float DeltaMovement);

	void RotateStalker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	APawnStalkerTurret();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetStalkerEnabledState(bool bEnabled);
};
