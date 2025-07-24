// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "ItemTableRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct GENERICGAEM_API FItemTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<ABaseItem> Item;
};
