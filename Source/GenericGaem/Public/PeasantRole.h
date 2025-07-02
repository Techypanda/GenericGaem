// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseRole.h"

/**
 * 
 */
class GENERICGAEM_API PeasantRole : public BaseRole
{
public:
	static constexpr std::string_view RoleName = "Peasant";
	static constexpr std::string_view RoleDescription = "You have no moni, no power and monkeys will probably kill you. Atleast you don't pay tax!";
	static constexpr std::string_view RolePrice = "0";
	PeasantRole();
	const FColor GetRoleColor() const override;
};
