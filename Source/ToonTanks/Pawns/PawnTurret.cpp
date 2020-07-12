// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h" // UGameplayStatics
#include "PawnTank.h"

APawnTurret::APawnTurret()
{

}

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();
	
    // Binding CheckFireCondition to the timer
    GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this,
        &APawnTurret::CheckFireCondition, FireRate, true, false);

    PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APawnTurret::CheckFireCondition() const
{
    // Check if we have a valid reference to the player.
    if (!PlayerPawn)
        return;

    // TODO: Check if player is dead (to be implemented in PawnTank class)

    // If player IS in range -> fire!!!
    if (ReturnDistanceToPlayer() <= FireRange)
    {
        // TODO: Fire (to be implemented in PawnBase class)
        UE_LOG(LogTemp, Warning, TEXT("%s FIRE!"), *GetName());
    }
}

float APawnTurret::ReturnDistanceToPlayer() const
{
    // Check if we have a valid reference to the player.
    if (!PlayerPawn)
        return 0.f;
    
    // Formula to get distance between the tank and this turret
    return (PlayerPawn->GetActorLocation() - GetActorLocation()).Size();
}