// Fill out your copyright notice in the Description page of Project Settings.


#include "PeasantRole.h"

UPeasantRole::UPeasantRole() : UBaseRole(UPeasantRole::RoleName, UPeasantRole::RoleDescription, UPeasantRole::RolePrice)
{
}

const FColor UPeasantRole::GetRoleColor() const
{
	return FColor{ 115, 74, 22, 255 };
}
