// Fill out your copyright notice in the Description page of Project Settings.


#include "MonkeyRole.h"

const TArray<std::string_view> UMonkeyRole::StarterItems = {};

UMonkeyRole::UMonkeyRole() : UBaseRole(UMonkeyRole::RoleName, UMonkeyRole::RoleDescription, UMonkeyRole::RolePrice, UMonkeyRole::StarterItems)
{
}

const FColor UMonkeyRole::GetRoleColor() const
{
	return FColor{ 255, 165, 0, 255 }; // Orange color for monkeys (swap to brown but peasant is brown... to think on)
}