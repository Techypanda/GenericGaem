// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRole.h"
#include "BaseRole.generated.h"

/**
 * 
 */
UCLASS()
class GENERICGAEM_API UBaseRole : public UObject, public IRole
{
	GENERATED_BODY()
private:
	std::string_view _RoleName, _RoleDescription, _RolePrice;
public:
	static constexpr std::string_view BaseRoleName = "None";
	static constexpr std::string_view BaseRoleDescription = "No Role Selected";
	static constexpr std::string_view BaseRolePrice = "0";
	UBaseRole();
	UBaseRole(const std::string_view InRoleName, const std::string_view InRoleDescription, const std::string_view InRolePrice);
	virtual const FColor GetRoleColor() const override;
	virtual const std::string_view GetRoleName() const override;
	virtual const std::string_view GetRoleDescription() const override;
	virtual const std::string_view GetRolePrice() const override;
};
