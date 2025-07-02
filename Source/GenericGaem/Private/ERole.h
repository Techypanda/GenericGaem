// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRole.h"
#include <memory>
#include <vector>

/**
 * 
 */
UENUM()
enum class ERole : uint8
{
	None = 0,
	Peasant,
	Citizen,
	Leader
};

class ERoleHelper
{
public:
	static std::unique_ptr<IRole> ERoleToRole(ERole InRole);
	static std::vector<std::unique_ptr<IRole>> GetAllRoles();
};