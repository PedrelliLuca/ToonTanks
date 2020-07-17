// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

APawnTank::APawnTank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm); // So that we get control over camera distance

}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();
	
    PlayerController = Cast<APlayerController>(GetController());
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Moving tank
    Rotate();
    Move();

    // Rotating tank turret
    if(PlayerController)
    {
        PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
        RotateTurret(TraceHitResult.ImpactPoint);
    }
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
    PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotationInput);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);
}

void APawnTank::CalculateMoveInput(float Value)
{
    // Value > 0: forward; Value < 0: backward;
    //
    // Multiplying by GetWorld()->DeltaTimeSeconds makes the movement framerate independent.
    //
    // Y and Z components are set to 0 because our tank is only supposed to move forward and
    // backward based on its rotation.
    MoveDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);;
}

void APawnTank::CalculateRotationInput(float Value)
{
    // Value > 0: right; Value < 0: left
    float RotationAmount = Value * RotationSpeed * GetWorld()->DeltaTimeSeconds;
    FRotator Rotation(0, RotationAmount, 0);
    RotationDirection = FQuat(Rotation);
}

void APawnTank::Move()
{
    // Adds a delta to the location of this component in its local reference frame.
    // The second parameter (sweep) is to set if collisions should be checked when moving.
    AddActorLocalOffset(MoveDirection, true);
}

void APawnTank::Rotate()
{
    // Similar to AddActorLocalOffset
    AddActorLocalRotation(RotationDirection, true);
}

void APawnTank::HandleDestruction()
{
    Super::HandleDestruction(); // We first execute the universal functionalities

    bAlive = false;

    // We can't destroy the pawn controlled by the player, this would lead to a ton of bugs and
    // potential errors. Therefore, we only give the illusion that the tank was destroyed by:
    // 1) Hiding visual components
    SetActorHiddenInGame(true);
    // 2) Disable inputs
    SetActorTickEnabled(false);
    // 3) Disable collisions
    SetActorEnableCollision(false);
}

bool APawnTank::GetPlayerAlive() const
{
    return bAlive;
}