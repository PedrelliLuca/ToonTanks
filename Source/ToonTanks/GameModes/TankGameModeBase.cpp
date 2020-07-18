// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"

void ATankGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    
    TargetTurrets = GetTargetTurretCount();
    PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
    PlayerControllerRef = 
        Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));
    HandleGameStart(); 
}

// Function to get the number of turrets in the world.
int32 ATankGameModeBase::GetTargetTurretCount()
{
    TSubclassOf<APawnTurret> ClassToFind;
    ClassToFind = APawnTurret::StaticClass();
    TArray<AActor*> TurretActors;
    UGameplayStatics::GetAllActorsOfClass(this, ClassToFind, TurretActors);

    // Calculate max score for this game
    for (AActor* Turret : TurretActors)
        if (APawnTurret* ActuallyTurret = Cast<APawnTurret>(Turret))
            MaximumScore += ActuallyTurret->GetTurretScore();

    return TurretActors.Num();
}

void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
    if (DeadActor == PlayerTank)
    {
        PlayerTank->PawnDestroyed();
        HandleGameOver(false); // the player didn't win

        if (PlayerControllerRef)
            PlayerControllerRef->SetPlayerEnabledState(false); // Player cannot move anymore
    }
    // If the following cast is successful it means that DeadActor is a APawnTurret ptr (dynamic casting)
    else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
    {
        PlayerScore += DestroyedTurret->GetTurretScore();
        DestroyedTurret->PawnDestroyed();
        TargetTurrets--;
        if(TargetTurrets == 0)
            HandleGameOver(true); // the player won!
    }
}

void ATankGameModeBase::HandleGameStart()
{
    GameStart(); // BP function
    if (PlayerControllerRef)
    {
        // Player movement disabled as soon as we start playing
        PlayerControllerRef->SetPlayerEnabledState(false);

        // We now need to wait for the countdown duration (StartDelay) and call the function again
        // with true

        FTimerHandle PlayerEnableHandle;
        // Allows us to create a temporary UObject with our function override
        // The object is of class PlayerControllerRef, the function is SetPlayerEnabledState, with
        // argument true
        FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(
            PlayerControllerRef,
            &APlayerControllerBase::SetPlayerEnabledState,
            true
        );

        // This is a different SetTimer overload compared to the one in PawnTurret.cpp
        // We're using this because SetPlayerEnabledState is a function with a parameter.
        GetWorldTimerManager().SetTimer(
            PlayerEnableHandle,
            PlayerEnableDelegate,
            StartDelay,
            false // No looping
        );

    }
}

void ATankGameModeBase::HandleGameOver(bool bPlayerWon)
{
    // If 0 turrets are left, show win result.
    // If tank was destroyed, show lose result.
    GameOver(bPlayerWon, PlayerScore, MaximumScore); // The other BP function
}
