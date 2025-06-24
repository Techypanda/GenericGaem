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
	std::string_view RoleName;
public:
	static const std::string_view BaseRoleName;
	BaseRole(const std::string_view InRoleName = BaseRoleName);
	virtual ~BaseRole() override;
	virtual const std::string_view GetRoleName() const override;
};
