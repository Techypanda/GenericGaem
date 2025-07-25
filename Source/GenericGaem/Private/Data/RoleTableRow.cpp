// Fill out your copyright notice in the Description page of Project Settings.

#include "RoleTableRow.h"
#include "DataRole.h"
#include "RoleName.h"
const TArray<UDataRole*> FRoleTableRow::GetAllRoles(UDataTable* RoleTable)
{
	TArray<FName> RowNames = RoleTable->GetRowNames();
	TArray<UDataRole*> OutArray{};
	for (const auto& _RowName : RowNames)
	{
		OutArray.Add(UDataRole::CreateUDataRoleFromRow(_RowName.ToString(), RoleTable->FindRow<FRoleTableRow>(_RowName, TEXT("FRoleTableRow::GetAllRoles"), true)));
	}
	return OutArray;
}

const UDataRole* FRoleTableRow::GetDataRole(FString RoleName, UDataTable* RoleTable)
{
	return UDataRole::CreateUDataRoleFromRow(RoleName, RoleTable->FindRow<FRoleTableRow>(FName(*RoleName), TEXT("FRoleTableRow::GetDataRole"), true));
}

void FRoleTableRow::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
	UE_LOG(LogTemp, Warning, TEXT("FRoleTableRow::OnDataTableChanged called for row: %s"), *InRowName.ToString());
	if (ERoleHelper::StringToERoleName.Contains(InRowName.ToString()))
	{
		RoleName = ERoleHelper::StringToERoleName[InRowName.ToString()];
	}
	else
	{
		RoleName = ERoleName::None;
	}
}

EDataValidationResult FRoleTableRow::IsDataValid(FDataValidationContext& Context) const
{
	return ERoleHelper::ERoleNameToString.Contains(RoleName) && RoleName != ERoleName::None ? EDataValidationResult::Valid : EDataValidationResult::Invalid;
}
