// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseRole.h"

/**
 * 
 */
class GENERICGAEM_API MonkeyRole : public BaseRole
{
public:
	static constexpr std::string_view RoleName = "Monkey";
	static constexpr std::string_view RoleDescription = "kill human, eat banana, oo oo aaa aaa";
	static constexpr std::string_view RolePrice = "0";
	MonkeyRole();
	const FColor GetRoleColor() const override;
};
