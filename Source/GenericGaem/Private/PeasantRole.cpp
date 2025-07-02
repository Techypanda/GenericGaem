// Fill out your copyright notice in the Description page of Project Settings.


#include "PeasantRole.h"

PeasantRole::PeasantRole() : BaseRole(PeasantRole::RoleName, PeasantRole::RoleDescription, PeasantRole::RolePrice)
{
}

const FColor PeasantRole::GetRoleColor() const
{
	return FColor{ 115, 74, 22, 255 };
}
