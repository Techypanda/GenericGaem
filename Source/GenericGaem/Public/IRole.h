// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <string_view>

/**
 * 
 */
class GENERICGAEM_API IRole
{
public:
	IRole();
	virtual ~IRole();
	virtual const std::string_view GetRoleName() const = 0;
};
