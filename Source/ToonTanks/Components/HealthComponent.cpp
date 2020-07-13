// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "ToonTanks/GameModes/TankGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Health = DefaultHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	GameModeRef = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	Owner = GetOwner();
	if(Owner)
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	else
		UE_LOG(LogTemp, Error, TEXT("Health component does not have any reference to the owner!"))	
}

// Thanks to dynamic binding, this is automatically called whenever the actor is damaged in any way
void UHealthComponent::TakeDamage(
	AActor* DamagedActor,
	float Damage,
	const class UDamageType* DamageType,
	class AController* InstigatedBy,
	AActor* DamageCauser
)
{
	if (Damage == 0 || Health == 0) 
		return;

	Health = FMath::Clamp(Health-Damage, 0.f, DefaultHealth);

	if (Health <= 0)
		if (GameModeRef)
			// Called only once thanks to the Health == 0 check above.
			GameModeRef->ActorDied(Owner);
		else
			UE_LOG(LogTemp, Error, TEXT("Health component does not have any reference to the GameMode!"));
}