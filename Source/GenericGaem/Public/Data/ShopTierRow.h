// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShopTierRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct GENERICGAEM_API FShopTierRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	int TierNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	FString Price;
};
