// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseRole.h"
#include "RoleTableRow.h"
#include "DataRole.generated.h"

/**
 * 
 */
UCLASS()
class GENERICGAEM_API UDataRole : public UBaseRole
{
	GENERATED_BODY()
public:
	static UDataRole* CreateUDataRoleFromRow(const FString RoleName, const FRoleTableRow* Row);
	UDataRole();
	const FColor GetRoleColor() const override;
protected:
	FRoleTableRow _Row;
};
