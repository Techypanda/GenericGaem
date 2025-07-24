// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "StarterItemsTableRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct GENERICGAEM_API FStarterItemsTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<TSubclassOf<class ABaseItem>> StartingItems;
};
