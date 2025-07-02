// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRole.h"

/**
 * 
 */
class GENERICGAEM_API BaseRole : public IRole
{
private:
	std::string_view _RoleName, _RoleDescription, _RolePrice;
public:
	static constexpr std::string_view BaseRoleName = "None";
	static constexpr std::string_view BaseRoleDescription = "No Role Selected";
	static constexpr std::string_view BaseRolePrice = "0";
	BaseRole(const std::string_view InRoleName = BaseRoleName, const std::string_view InRoleDescription = BaseRoleDescription, const std::string_view InRolePrice = BaseRolePrice);
	virtual ~BaseRole() override;
	virtual const FColor GetRoleColor() const override;
	virtual const std::string_view GetRoleName() const override;
	virtual const std::string_view GetRoleDescription() const override;
	virtual const std::string_view GetRolePrice() const override;
};
