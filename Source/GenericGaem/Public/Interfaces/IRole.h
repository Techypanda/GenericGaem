// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string_view>
#include "CoreMinimal.h"
#include "Math/Color.h"
#include "UObject/Interface.h"
#include "IRole.generated.h"


UINTERFACE(MinimalAPI, Blueprintable)
class URole : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GENERICGAEM_API IRole
{
	GENERATED_BODY()
public:
	virtual const std::string_view GetRoleName() const = 0;
	virtual const FColor GetRoleColor() const = 0;
	virtual const std::string_view GetRoleDescription() const = 0;
	virtual const std::string_view GetRolePrice() const = 0;
};
