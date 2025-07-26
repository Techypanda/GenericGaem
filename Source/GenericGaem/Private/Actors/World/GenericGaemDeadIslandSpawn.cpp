// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemDeadIslandSpawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"

// Sets default values
AGenericGaemDeadIslandSpawn::AGenericGaemDeadIslandSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SpawnForwardVector = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnForwardVector"));
	SetRootComponent(SpawnForwardVector);
}

FVector AGenericGaemDeadIslandSpawn::GetSpawnLocation() const
{
	FVector SpawnLocation = GetActorLocation();
	const float MinX = SpawnLocation.X - SpawnRadius, MaxX = SpawnLocation.X + SpawnRadius;
	const float MinY = SpawnLocation.Y - SpawnRadius, MaxY = SpawnLocation.Y + SpawnRadius;
	SpawnLocation.X = UKismetMathLibrary::RandomFloatInRange(MinX, MaxX);
	SpawnLocation.Y = UKismetMathLibrary::RandomFloatInRange(MinY, MaxY);
	SpawnLocation.Z += 25.0f; // Slightly above the ground
	return SpawnLocation;
}

// doesn't work tbd
FRotator AGenericGaemDeadIslandSpawn::GetSpawnRotation() const
{
	return FRotator();
}
