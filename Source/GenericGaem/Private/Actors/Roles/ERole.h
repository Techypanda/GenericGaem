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
	Monkey,
	Leader
};

class ERoleHelper
{
public:
	static IRole* ERoleToRole(ERole InRole);
	static std::vector<IRole*> GetAllRoles();
};