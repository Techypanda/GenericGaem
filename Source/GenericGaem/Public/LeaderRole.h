// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseRole.h"
#include "LeaderRole.generated.h"

/**
 * 
 */
UCLASS()
class GENERICGAEM_API ULeaderRole : public UBaseRole
{
	GENERATED_BODY()
public:
	static constexpr std::string_view RoleName = "Leader";
	static constexpr std::string_view RoleDescription = "Rule over people, probably corrupt, probably a nice guy, kill people?";
	static constexpr std::string_view RolePrice = "99999999999999999"; // You should not be buying this role
	ULeaderRole();
	const FColor GetRoleColor() const override;
};
