// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemListItemWidget.h"
#include "GenericGaemPlayerState.h"
#include "RoleTableRow.h"
#include "Shop.h"
#include "DataRole.h"
#include <string>

void UGenericGaemListItemWidget::SetShop(AShop* Shop)
{
	_Shop = Shop;
	PurchaseProgress = 0.0f;
}

bool UGenericGaemListItemWidget::SelectRole(FString RoleName)
{
	const auto& RequestedRole = FRoleTableRow::GetDataRole(RoleName, RoleTable);
	const auto PlayerState = GetOwningPlayerState<AGenericGaemPlayerState>();
	const auto Price = FCString::Atoi(*RequestedRole->GetRolePrice());
	const auto PlayerMoney = FCString::Atoi(*PlayerState->GetMoney());
	const auto CanPurchase = PlayerMoney >= Price;
	UE_LOG(LogTemp, Warning, TEXT("Player Money: %d, Role Price: %d, %hs"), PlayerMoney, Price, CanPurchase ? "can purchase" : "can't purchase");
	// client side validation to avoid wasteful RPC, serverside will also do this
	if (CanPurchase) {
		PlayerState->ServerPurchaseRole(RoleName);
		PlayerState->OnRolePurchased().Broadcast();
	}
	return false;
}

bool UGenericGaemListItemWidget::BeginPurchaseItem(FString RowName, FString Price)
{
	const auto PlayerState = GetOwningPlayerState<AGenericGaemPlayerState>();
	if (FCString::Atoi(*PlayerState->GetMoney()) < FCString::Atoi(*Price))
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough money to purchase item %s. Required: %s, Available: %s"), *RowName, *Price, *PlayerState->GetMoney());
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("Player %s has enough money to purchase item %s. Price: %s"), *PlayerState->GetPlayerName(), *RowName, *Price);
	PlayerState->ServerBeginPurchasingItem(5.0f, RowName, _Shop); // 5 seconds to purchase item
	return true;
}
