// Fill out your copyright notice in the Description page of Project Settings.


#include "CitizenRole.h"

UCitizenRole::UCitizenRole() : UBaseRole(UCitizenRole::RoleName, UCitizenRole::RoleDescription, UCitizenRole::RolePrice)
{
}

const FColor UCitizenRole::GetRoleColor() const
{
	return FColor{ 0, 0, 255, 255 };
}
