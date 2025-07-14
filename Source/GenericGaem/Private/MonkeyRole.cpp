// Fill out your copyright notice in the Description page of Project Settings.


#include "MonkeyRole.h"

MonkeyRole::MonkeyRole() : BaseRole(MonkeyRole::RoleName, MonkeyRole::RoleDescription, MonkeyRole::RolePrice)
{
}

const FColor MonkeyRole::GetRoleColor() const
{
	return FColor{ 255, 165, 0, 255 }; // Orange color for monkeys (swap to brown but peasant is brown... to think on)
}