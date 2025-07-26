// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemRoleSpawnpoint.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"

// Sets default values
AGenericGaemRoleSpawnpoint::AGenericGaemRoleSpawnpoint() : SpawnRadius{ 5.0f }
{
	PrimaryActorTick.bCanEverTick = false;
	SpawnForwardVector = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnForwardVector"));
	SetRootComponent(SpawnForwardVector);
}

FVector AGenericGaemRoleSpawnpoint::GetSpawnLocation() const
{
	FVector SpawnLocation = GetActorLocation();
	const float MinX = SpawnLocation.X - SpawnRadius, MaxX = SpawnLocation.X + SpawnRadius;
	const float MinY = SpawnLocation.Y - SpawnRadius, MaxY = SpawnLocation.Y + SpawnRadius;
	SpawnLocation.X = UKismetMathLibrary::RandomFloatInRange(MinX, MaxX);
	SpawnLocation.Y = UKismetMathLibrary::RandomFloatInRange(MinY, MaxY);
	SpawnLocation.Z += 25.0f; // Slightly above the ground
	return SpawnLocation;
}

FRotator AGenericGaemRoleSpawnpoint::GetSpawnRotation() const
{
	return SpawnForwardVector->GetForwardVector().Rotation();
}
