// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthPack.generated.h"

class APawnTank;

UCLASS()
class TOONTANKS_API AHealthPack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPack();

	// Called every frame
	// virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HealthPackMesh = nullptr;

	// VARIABLES AND PTRS
	APawnTank* PlayerTank = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing", meta = (AllowPrivateAccess = "true"))
	float Healing = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Healing", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageType;

	// EFFECTS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* HitParticle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	USoundBase* HitSound = nullptr;

	// FUNCTIONS
	UFUNCTION() // Dynamic delegates always need to be declare UFUNCTION()
	void OnHit( // The arguments of the delegate must be the same of the original function
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);
};
