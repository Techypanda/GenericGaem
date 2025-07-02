// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRole.h"

BaseRole::BaseRole(const std::string_view InRoleName, const std::string_view InRoleDescription, const std::string_view InRolePrice) : _RoleName(InRoleName), _RoleDescription(InRoleDescription), _RolePrice(InRolePrice)
{
}

BaseRole::~BaseRole()
{
}

const FColor BaseRole::GetRoleColor() const
{
	return FColor::Black;
}

const std::string_view BaseRole::GetRoleName() const
{
	return _RoleName;
}

const std::string_view BaseRole::GetRoleDescription() const
{
	return _RoleDescription;
}

const std::string_view BaseRole::GetRolePrice() const
{
	return _RolePrice;
}
