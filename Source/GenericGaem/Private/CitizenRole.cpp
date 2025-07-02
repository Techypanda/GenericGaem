// Fill out your copyright notice in the Description page of Project Settings.


#include "CitizenRole.h"

CitizenRole::CitizenRole() : BaseRole(CitizenRole::RoleName, CitizenRole::RoleDescription, CitizenRole::RolePrice)
{
}

const FColor CitizenRole::GetRoleColor() const
{
	return FColor{ 0, 0, 255, 255 };
}
