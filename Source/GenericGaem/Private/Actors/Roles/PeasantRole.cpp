// Fill out your copyright notice in the Description page of Project Settings.


#include "PeasantRole.h"

const TArray<std::string_view> UPeasantRole::StarterItems = {};

UPeasantRole::UPeasantRole() : UBaseRole(UPeasantRole::RoleName, UPeasantRole::RoleDescription, UPeasantRole::RolePrice, UPeasantRole::StarterItems)
{
}

const FColor UPeasantRole::GetRoleColor() const
{
	return FColor{ 115, 74, 22, 255 };
}
