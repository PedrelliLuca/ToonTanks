// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankGameModeBase.generated.h"

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
	void GameOver(bool bPlayerWon);

protected:
	// Collects relevant infos from map when the game starts
	virtual void BeginPlay() override;

private:
	void HandleGameStart();

	void HandleGameOver(bool bPlayerWon);
};
