// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRole.h"
#include "BaseItem.h"

const TArray<TSubclassOf<ABaseItem>> UBaseRole::BaseStarterItems = {};
FString UBaseRole::BaseRoleName = "None";
FString UBaseRole::BaseRoleDescription = "No Role Selected";
FString UBaseRole::BaseRolePrice = "0";
FString UBaseRole::LeaderRoleName = "Leader";

UBaseRole::UBaseRole() : UBaseRole(BaseRoleName, BaseRoleDescription, BaseRolePrice, BaseStarterItems)
{
}

UBaseRole::UBaseRole(const FString InRoleName, const FString InRoleDescription, const FString InRolePrice, const TArray<TSubclassOf<class ABaseItem>> InStarterItems) : _StarterItems(InStarterItems), _RoleName(InRoleName), _RoleDescription(InRoleDescription), _RolePrice(InRolePrice)
{
}

const FColor UBaseRole::GetRoleColor() const
{
	return FColor::Black;
}

const FString UBaseRole::GetRoleName() const
{
	return _RoleName;
}

const FString UBaseRole::GetRoleDescription() const
{
	return _RoleDescription;
}

const FString UBaseRole::GetRolePrice() const
{
	return _RolePrice;
}

const TArray<TSubclassOf<class ABaseItem>> UBaseRole::GetStarterItems() const
{
	return _StarterItems;
}
