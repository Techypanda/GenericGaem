// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericGaemDeadIslandSpawn.generated.h"

UCLASS()
class AGenericGaemDeadIslandSpawn : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AGenericGaemDeadIslandSpawn();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UArrowComponent* SpawnForwardVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
	float SpawnRadius;
	UFUNCTION(BlueprintCallable)
	FVector GetSpawnLocation() const;
	UFUNCTION(BlueprintCallable)
	FRotator GetSpawnRotation() const;
};
