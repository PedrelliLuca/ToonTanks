// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankGameModeBase.generated.h"

class APawnTank;
class APawnTurret;
class APlayerControllerBase;

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// Will be called by the tank or the turret and then the GameMode will decide whether or not
	// the game should end.
	void ActorDied(AActor* DeadActor);

	UFUNCTION(BlueprintImplementableEvent) // To handle functionalities in derived blueprint class
	void GameStart();

	UFUNCTION(BlueprintImplementableEvent) // To handle functionalities in derived blueprint class
	void GameOver(bool bPlayerWon, int32 PlayerPoints, int32 MaxPoints);

protected:
	// Collects relevant infos from map when the game starts
	virtual void BeginPlay() override;

private:
	int32 TargetTurrets = 0;

	// Variable to control starting countdown duration. int because int32 isn't accepted by BPs.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
	int StartDelay = 3; 

	int32 MaximumScore = 0;
	int32 PlayerScore = 0;

	UPROPERTY(
		VisibleAnywhere, 
		BlueprintReadOnly, 
		Category = "Player", 
		meta = (AllowPrivateAccess = "true")
	)
	APawnTank* PlayerTank = nullptr;

	APlayerControllerBase* PlayerControllerRef = nullptr;

	void HandleGameStart();
	void HandleGameOver(bool bPlayerWon);
	int32 GetTargetTurretCount();
};
