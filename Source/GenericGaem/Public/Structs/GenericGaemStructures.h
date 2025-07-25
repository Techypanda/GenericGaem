// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericGaemStructures.generated.h"

USTRUCT(BlueprintType)
struct GENERICGAEM_API FRoleListItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoleListItemProperties")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoleListItemProperties")
	FString Price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoleListItemProperties")
	FString Description;
};
