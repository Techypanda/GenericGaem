// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoleName.h"
#include "Misc/DataValidation.h"
#include "RoleTableRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct GENERICGAEM_API FRoleTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
	FString Price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
	TArray<TSubclassOf<class ABaseItem>> StartingItems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
	FColor Color;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
	TEnumAsByte<ERoleName> RoleName;

	static const TArray<class UDataRole*> GetAllRoles(UDataTable* RoleTable);
	static const class UDataRole* GetDataRole(FString RoleName, UDataTable* RoleTable);

	void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
	EDataValidationResult IsDataValid(FDataValidationContext& Context) const;
};
