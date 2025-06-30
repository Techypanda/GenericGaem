// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaderRole.h"

const std::string_view LeaderRole::RoleName = "Leader";

LeaderRole::LeaderRole() : BaseRole(LeaderRole::RoleName)
{
}

const FColor LeaderRole::GetRoleColor() const
{
	return FColor{ 202, 11, 0, 255 };
}