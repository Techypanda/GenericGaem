// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRole.h"

UBaseRole::UBaseRole() : UBaseRole(BaseRoleName, BaseRoleDescription, BaseRolePrice)
{
}

UBaseRole::UBaseRole(const std::string_view InRoleName, const std::string_view InRoleDescription, const std::string_view InRolePrice) : _RoleName(InRoleName), _RoleDescription(InRoleDescription), _RolePrice(InRolePrice)
{
}

const FColor UBaseRole::GetRoleColor() const
{
	return FColor::Black;
}

const std::string_view UBaseRole::GetRoleName() const
{
	return _RoleName;
}

const std::string_view UBaseRole::GetRoleDescription() const
{
	return _RoleDescription;
}

const std::string_view UBaseRole::GetRolePrice() const
{
	return _RolePrice;
}
