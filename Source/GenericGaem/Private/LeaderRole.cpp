// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaderRole.h"

LeaderRole::LeaderRole() : BaseRole(LeaderRole::RoleName, LeaderRole::RoleDescription, LeaderRole::RolePrice)
{
}

const FColor LeaderRole::GetRoleColor() const
{
	return FColor{ 202, 11, 0, 255 };
}