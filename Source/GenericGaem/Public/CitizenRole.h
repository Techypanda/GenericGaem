// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseRole.h"

/**
 * 
 */
class GENERICGAEM_API CitizenRole : public BaseRole
{
public:
	static constexpr std::string_view RoleName = "Citizen";
	static constexpr std::string_view RoleDescription = "Lowerclass of citizen, not a gutter rat but close, work your way up in the town";
	static constexpr std::string_view RolePrice = "250";
	CitizenRole();
	const FColor GetRoleColor() const override;
};
