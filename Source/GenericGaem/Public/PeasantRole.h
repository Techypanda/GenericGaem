// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseRole.h"
#include "PeasantRole.generated.h"

/**
 * 
 */
UCLASS()
class GENERICGAEM_API UPeasantRole : public UBaseRole
{
	GENERATED_BODY()
public:
	static constexpr std::string_view RoleName = "Peasant";
	static constexpr std::string_view RoleDescription = "You have no moni, no power and monkeys will probably kill you. Atleast you don't pay tax!";
	static constexpr std::string_view RolePrice = "0";
	UPeasantRole();
	const FColor GetRoleColor() const override;
};
