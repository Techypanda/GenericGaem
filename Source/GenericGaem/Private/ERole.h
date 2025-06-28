// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRole.h"
#include <memory>

/**
 * 
 */
UENUM()
enum class ERole : uint8
{
	None,
	Leader,
	Peasant
};

class ERoleHelper
{
public:
	static std::unique_ptr<IRole> ERoleToRole(ERole InRole);
};