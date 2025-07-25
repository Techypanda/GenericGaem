// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemListItemWidget.h"
#include "GenericGaemPlayerState.h"
#include "RoleTableRow.h"
#include "DataRole.h"
#include <string>

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