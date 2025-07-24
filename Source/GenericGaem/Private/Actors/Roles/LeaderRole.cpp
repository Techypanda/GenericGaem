// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaderRole.h"

const TArray<std::string_view> ULeaderRole::StarterItems = {};

ULeaderRole::ULeaderRole() : UBaseRole(ULeaderRole::RoleName, ULeaderRole::RoleDescription, ULeaderRole::RolePrice, ULeaderRole::StarterItems)
{
}

const FColor ULeaderRole::GetRoleColor() const
{
	return FColor{ 202, 11, 0, 255 };
}