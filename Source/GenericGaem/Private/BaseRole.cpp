// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRole.h"

const std::string_view BaseRole::BaseRoleName = "None";

BaseRole::BaseRole(const std::string_view InRoleName) : RoleName(InRoleName)
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
	return RoleName;
}