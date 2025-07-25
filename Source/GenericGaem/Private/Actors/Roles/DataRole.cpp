// Fill out your copyright notice in the Description page of Project Settings.


#include "DataRole.h"

UDataRole* UDataRole::CreateUDataRoleFromRow(const FString RoleName, const FRoleTableRow* Row)
{
	const auto& InitializedRole = NewObject<UDataRole>();
	if (!Row)
	{
		return InitializedRole;
	}
	InitializedRole->_RoleDescription = Row->Description;
	InitializedRole->_RolePrice = Row->Price;
	InitializedRole->_StarterItems = Row->StartingItems;
	InitializedRole->_RoleName = RoleName;
	InitializedRole->_Row = *Row;
	return InitializedRole;
}

UDataRole::UDataRole() : Super()
{
}

const FColor UDataRole::GetRoleColor() const
{
	return _Row.Color;
}
