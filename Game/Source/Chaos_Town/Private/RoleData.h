// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoleData.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FRoleData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AGameCharacter> RoleCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Price;
};
