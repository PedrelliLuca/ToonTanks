// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnTurret.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API APawnTurret : public APawnBase
{
	GENERATED_BODY()

public:

	APawnTurret();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	// Function which will be called on a repeating timer.
	void CheckFireCondition();

	// How often we call CheckFireCondition()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", 
		meta = (AllowPrivateAccess = "true"))
	float FireRate = 2.f;

	// Timer to bind and unbind CheckFireCondition() at runtime.
	FTimerHandle FireRateTimerHandle;
};
