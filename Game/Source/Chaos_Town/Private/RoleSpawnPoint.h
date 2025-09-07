// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RoleSpawnPoint.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URoleSpawnPoint : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IRoleSpawnPoint
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	const FName GetRoleName();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	const FVector GetSpawnLocation();
};
