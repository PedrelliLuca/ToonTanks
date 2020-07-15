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
    GetWorld()->GetTimerManager().SetTimer(
        FireRateTimerHandle,
        this,
        &APawnTurret::CheckFireCondition,
        FireRate,
        true,
        false
    );

    PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(!PlayerPawn)
        return;
    
    RotateTurret(PlayerPawn->GetActorLocation());
}

void APawnTurret::CheckFireCondition()
{
    // If we don't have a valid reference to the player or he lost, we don't do anything.
    if (!PlayerPawn || !PlayerPawn->GetPlayerAlive())
        return;

    // If player IS in range -> fire!!!
    if (ReturnDistanceToPlayer() <= FireRange)
        Fire();
}

float APawnTurret::ReturnDistanceToPlayer() const
{
    // Check if we have a valid reference to the player.
    if (!PlayerPawn)
        return 0.f;
    
    // Formula to get distance between the tank and this turret
    return (PlayerPawn->GetActorLocation() - GetActorLocation()).Size();
}

void APawnTurret::HandleDestruction()
{
    Super::HandleDestruction(); // We first execute the universal functionalities

    // Turret-specific functionalities
    Destroy();
}
