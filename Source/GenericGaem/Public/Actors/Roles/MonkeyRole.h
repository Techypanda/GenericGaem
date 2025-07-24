// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseRole.h"
#include "MonkeyRole.generated.h"

/**
 * 
 */
UCLASS()
class GENERICGAEM_API UMonkeyRole : public UBaseRole
{
	GENERATED_BODY()
public:
	static constexpr std::string_view RoleName = "Monkey";
	static constexpr std::string_view RoleDescription = "kill human, eat banana, oo oo aaa aaa";
	static constexpr std::string_view RolePrice = "0";
	UMonkeyRole();
	const FColor GetRoleColor() const override;
};
