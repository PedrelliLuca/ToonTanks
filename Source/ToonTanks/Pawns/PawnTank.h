// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnTank.generated.h"

class USpringArmComponent;
class UCameraComponent;
 
UCLASS()
class TOONTANKS_API APawnTank : public APawnBase
{
	GENERATED_BODY()
	
public:

	APawnTank();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera = nullptr;

	// Members for pawn movement
	FVector MoveDirection;
	FQuat RotationDirection;

	float MoveSpeed = 500.f;
	float RotationSpeed = 200.f;

	// Functions to calculate changes to tank movement and rotation.
	// Called everytime BindAxes is updated.
	void CalculateMoveInput(float Value);
	void CalculateRotationInput(float Value);

	// Functions to update movement and rotation of the tank
	void Move();
	void Rotate();

};
