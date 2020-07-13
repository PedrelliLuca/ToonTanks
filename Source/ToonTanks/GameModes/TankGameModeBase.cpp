// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"

void ATankGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    
    TargetTurrets = GetTargetTurretCount();
    PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
    HandleGameStart(); 
}

// Function to get the number of turrets in the world.
int32 ATankGameModeBase::GetTargetTurretCount() const
{
    TSubclassOf<APawnTurret> ClassToFind;
    ClassToFind = APawnTurret::StaticClass();
    TArray<AActor*> TurretActors;
    UGameplayStatics::GetAllActorsOfClass(this, ClassToFind, TurretActors);
    return TurretActors.Num();
}

void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
    if (DeadActor == PlayerTank)
    {
        PlayerTank->PawnDestroyed();
        HandleGameOver(false); // the player didn't win
    }
    // If the following cast is successful it means that DeadActor is a APawnTurret ptr (dynamic casting)
    else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
    {
        DestroyedTurret->PawnDestroyed();
        TargetTurrets--;
        if(TargetTurrets == 0)
            HandleGameOver(true); // the player won!
    }
}

void ATankGameModeBase::HandleGameStart()
{
    GameStart(); // BP function 
}

void ATankGameModeBase::HandleGameOver(bool bPlayerWon)
{
    // If 0 turrets are left, show win result.
    // If tank was destroyed, show lose result.
    GameOver(bPlayerWon); // The other BP function
}
