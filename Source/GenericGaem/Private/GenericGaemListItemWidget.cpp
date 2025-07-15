// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemListItemWidget.h"
#include "GenericGaemPlayerState.h"
#include "ERole.h"
#include <string>

bool UGenericGaemListItemWidget::SelectRole(int32 ERoleVal)
{
	const auto RequestedRole = ERoleHelper::ERoleToRole(static_cast<ERole>(ERoleVal));
	const auto PlayerState = GetOwningPlayerState<AGenericGaemPlayerState>();
	const auto Price = FCString::Atoi(*FString(RequestedRole->GetRolePrice().data()));
	const auto PlayerMoney = FCString::Atoi(*PlayerState->GetMoney());
	const auto CanPurchase = PlayerMoney >= Price;
	UE_LOG(LogTemp, Warning, TEXT("Player Money: %d, Role Price: %d, %hs"), PlayerMoney, Price, CanPurchase ? "can purchase" : "can't purchase");
	// client side validation to avoid wasteful RPC, serverside will also do this
	if (CanPurchase) {
		PlayerState->ServerPurchaseRole(static_cast<ERole>(ERoleVal));
		PlayerState->OnRolePurchased().Broadcast();
	}
	return CanPurchase;
}