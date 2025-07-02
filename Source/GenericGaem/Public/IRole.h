// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/Color.h"
#include <string_view>

/**
 * 
 */
class GENERICGAEM_API IRole
{
public:
	IRole() = default;
	virtual ~IRole() = default;
	virtual const std::string_view GetRoleName() const = 0;
	virtual const FColor GetRoleColor() const = 0;
	virtual const std::string_view GetRoleDescription() const = 0;
	virtual const std::string_view GetRolePrice() const = 0;
};
