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
protected:
	FString _RoleName, _RoleDescription, _RolePrice;
	TArray<TSubclassOf<class ABaseItem>> _StarterItems;
public:
	static FString BaseRoleName;
	static FString BaseRoleDescription;
	static FString BaseRolePrice;
	static FString LeaderRoleName;
	static const TArray<TSubclassOf<class ABaseItem>> BaseStarterItems;
	UBaseRole();
	UBaseRole(const FString InRoleName, const FString InRoleDescription, const FString InRolePrice, const TArray<TSubclassOf<class ABaseItem>> InStarterItems);
	virtual const FColor GetRoleColor() const override;
	virtual const FString GetRoleName() const override;
	virtual const FString GetRoleDescription() const override;
	virtual const FString GetRolePrice() const override;
	virtual const TArray<TSubclassOf<class ABaseItem>> GetStarterItems() const override;
};
