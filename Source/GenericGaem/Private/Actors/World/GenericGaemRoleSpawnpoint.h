// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ERole.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericGaemRoleSpawnpoint.generated.h"
UCLASS()
class AGenericGaemRoleSpawnpoint : public AActor
{
	GENERATED_BODY()
public:
	AGenericGaemRoleSpawnpoint();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UArrowComponent* SpawnForwardVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
	ERole RoleToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
	float SpawnRadius;
	UFUNCTION(BlueprintCallable)
	FVector GetSpawnLocation() const;
	UFUNCTION(BlueprintCallable)
	FRotator GetSpawnRotation() const;
};
