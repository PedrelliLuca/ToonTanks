// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"

void ATankGameModeBase::BeginPlay()
{
    // Get references and win/loss conditions.

    // Call HandleGameStart to initialize the start countdown, turret activation, pawn check etc.
}

void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
    UE_LOG(LogTemp, Warning, TEXT("Actor %s died!"), *DeadActor->GetName());
    // Check what type of actor died.
    // Tank -> go to lose condition
    // Turret -> tally.
}

void ATankGameModeBase::HandleGameStart()
{
    // initialize the start countdown, turret activation, pawn check etc.
    // Call BP version of GameStart()
}

void ATankGameModeBase::HandleGameOver(bool bPlayerWon)
{
    // If 0 turrets are left, show win result.
    // If tank was destroyed, show lose result.
    // Call BP version of GameOver()
}
