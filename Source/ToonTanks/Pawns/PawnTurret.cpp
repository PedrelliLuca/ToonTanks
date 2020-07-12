// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"

APawnTurret::APawnTurret()
{

}

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();
	
    // Binding CheckFireCondition to the timer
    GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandler, this,
        &APawnTurret::CheckFireCondition, FireRate, true, false);
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APawnTurret::CheckFireCondition()
{
    // If player == nullptr || player == Dead

    // If player IS in range -> fire!!!
}