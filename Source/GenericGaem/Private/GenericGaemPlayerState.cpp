// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemPlayerState.h"
#include "GenericGaemHUD.h"
#include "GenericGaemMode.h"
#include "Net/UnrealNetwork.h"
#include "GenericGaemCharacter.h"
#include "IItem.h"
#include "BaseItem.h"
#include "Engine/Engine.h"
#include "Engine/ActorChannel.h"

AGenericGaemPlayerState::AGenericGaemPlayerState() : _MoneyChangedEvent{}, _Health{ MaxHealth }, GameRoleLock{}, _EquippedItem{ nullptr }
{
	bReplicates = true;
}

void AGenericGaemPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGenericGaemPlayerState, _AssignedRole);
	DOREPLIFETIME(AGenericGaemPlayerState, _LastLeaderDateTimeString);
	DOREPLIFETIME(AGenericGaemPlayerState, bIsInvulnerable);
	DOREPLIFETIME(AGenericGaemPlayerState, _EquippedItem);
}

void AGenericGaemPlayerState::SetGameRole(ERole NewRole)
{
	FScopeLock Lock(&GameRoleLock);
	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting game role to %d for player_id: %d"), static_cast<int32>(NewRole), GetPlayerId());
		_AssignedRole = NewRole;
		OnGameRoleUpdate();
	}
}

ERole AGenericGaemPlayerState::GetGameRole() const
{
	return _AssignedRole;
}

FString AGenericGaemPlayerState::GetGameRoleAsString() const
{
	return ERoleHelper::ERoleToRole(GetGameRole())->GetRoleName().data();
}

FColor AGenericGaemPlayerState::GetGameRoleColor() const
{
	return ERoleHelper::ERoleToRole(GetGameRole())->GetRoleColor();
}

void AGenericGaemPlayerState::SetLastTimeLeader(FDateTime NewDateTime)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		_LastLeaderDateTimeString = NewDateTime.ToString();
		OnLastTimeLeaderUpdate();
	}
}

FDateTime AGenericGaemPlayerState::GetLastTimeLeaderAsDateTime() const
{
	FDateTime OutDateTime{};
	FDateTime::Parse(_LastLeaderDateTimeString, OutDateTime);
	return OutDateTime;
}

void AGenericGaemPlayerState::SetMoney(FString NewMoney)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		_Money = NewMoney;
		OnMoneyUpdate();
	}
}

FString AGenericGaemPlayerState::GetMoney() const
{
	return _Money;
}

void AGenericGaemPlayerState::ServerPurchaseRole_Implementation(ERole RoleToPurchase)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("PurchaseRole called on client, but this should only be called on the server!"));
		return;
	}
	const auto LocalRole = ERoleHelper::ERoleToRole(RoleToPurchase);
	if (!LocalRole)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid role requested for purchase: %d"), static_cast<int32>(RoleToPurchase));
		return;
	}
	const auto Price = FCString::Atoi(*FString(LocalRole->GetRolePrice().data()));
	const auto PlayerMoney = FCString::Atoi(*GetMoney());
	if (PlayerMoney < Price)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough money to purchase role %hs. Required: %d, Available: %d"),
			LocalRole->GetRoleName().data(), Price, PlayerMoney);
		return;
	}
	SetGameRole(RoleToPurchase);
	SetMoney(FString::FromInt(PlayerMoney - Price));
	GetWorld()->GetAuthGameMode<AGenericGaemMode>()->MovePlayerToSpawnPoint(this);
	UE_LOG(LogTemp, Warning, TEXT("Role %hs purchased successfully for player_id: %d. Remaining money: %s"),
		LocalRole->GetRoleName().data(), GetPlayerId(), *GetMoney());
}

void AGenericGaemPlayerState::SetHealth(float NewHealth)
{
	UE_LOG(LogTemp, Warning, TEXT("SetHealth called with NewHealth: %.2f for player_id: %d"), NewHealth, GetPlayerId());
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetHealth called on client, ignoring for player_id: %d"), GetPlayerId());
		return;
	}
	if (!bIsInvulnerable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting health for player_id: %d to %.2f"), GetPlayerId(), NewHealth);
		NewHealth = std::clamp(NewHealth, 0.0f, MaxHealth);
		if (NewHealth <= 0.0f) {
			const auto _Player = GetPawn<AGenericGaemCharacter>();
			_Player->ServerDeath();
		}
		_Health = NewHealth;
		OnHealthUpdate();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("SetHealth called player is invulnerable, ignoring for player_id: %d"), GetPlayerId());
	}
}

float AGenericGaemPlayerState::GetHealth() const
{
	return _Health;
}

void AGenericGaemPlayerState::SetInvulnerability(bool bInInvulnerability)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		bIsInvulnerable = bInInvulnerability;
		OnInvulnerabilityUpdate();
	}
}

bool AGenericGaemPlayerState::GetInvulnerability() const
{
	return bIsInvulnerable;
}

const TScriptInterface<IItem> AGenericGaemPlayerState::GetEquippedItem() const
{
	UE_LOG(LogTemp, Warning, TEXT("GetEquippedItem called for player_id: %d"), GetPlayerId());
	if (!_EquippedItem) {
		UE_LOG(LogTemp, Warning, TEXT("No equipped item for player_id: %d"), GetPlayerId());
		return nullptr;
	}
	if (!_EquippedItem->Implements<UItem>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Equipped item does not implement IItem interface for player_id: %d"), GetPlayerId());
		return nullptr;
	}
	return TScriptInterface<IItem>(_EquippedItem);
}

void AGenericGaemPlayerState::EquipItem(TScriptInterface<class IItem> ItemToEquip)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("EquipItem called on client, but this should only be called on the server!"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("No equipped item for player_id: %d"), GetPlayerId());
	if (!HasItemInInventory(ItemToEquip))
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to equip is not in inventory for player_id: %d"), GetPlayerId());
		return;
	}
	_EquippedItem = Cast<ABaseItem>(ItemToEquip.GetObject());
	UE_LOG(LogTemp, Warning, TEXT("Equipped item: %s for player_id: %d"), *(_EquippedItem ? _EquippedItem->GetName() : FString("None")), GetPlayerId());
}

void AGenericGaemPlayerState::AddToInventory(ABaseItem* Item)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddToInventory called on client, but this should only be called on the server!"));
		return;
	}
	if (_Inventory.Num() >= MaxInventorySize)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is full for player_id: %d, cannot add item: %s"), GetPlayerId(), *Item->GetName());
		return;
	}
	_Inventory.Add(Item);
	OnInventoryUpdate();
}

void AGenericGaemPlayerState::InventoryClear()
{
	for (int I = 0; I < _Inventory.Num(); I++)
	{
		const auto& ActorToDelete = _Inventory.Pop();
		ActorToDelete->Destroy();
	}
}

bool AGenericGaemPlayerState::HasItemInInventory(TScriptInterface<class IItem> ItemToEquip)
{
	// could optimize with a map here...
	for (const auto& _Item : _Inventory)
	{
		if (_Item && _Item->Implements<UItem>() && _Item == ItemToEquip.GetObject())
		{
			UE_LOG(LogTemp, Warning, TEXT("Item %s found in inventory for player_id: %d"), *_Item->GetName(), GetPlayerId());
			return true;
		}
	}
	return false;
}

void AGenericGaemPlayerState::OnRep_Inventory()
{
	OnInventoryUpdate();
}

void AGenericGaemPlayerState::OnInventoryUpdate()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("Server: Inventory Updated for player: %d, Inventory Size: %d"), GetPlayerId(), _Inventory.Num());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client: Inventory Updated for player: %d, Inventory Size: %d"), GetPlayerId(), _Inventory.Num());
	}
	_InventoryChangedEvent.Broadcast();
}

void AGenericGaemPlayerState::OnRep_Health()
{
	OnHealthUpdate();
}

void AGenericGaemPlayerState::OnRep_Invulnerability()
{
	OnInvulnerabilityUpdate();
}

void AGenericGaemPlayerState::OnInvulnerabilityUpdate()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("Server: Vulnerability updated to %hs"), bIsInvulnerable ? "is invulnerable" : "is vulnerable");
	}
}

void AGenericGaemPlayerState::OnHealthUpdate()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("Server: Health updated to %.2f for player_id: %d"), _Health, GetPlayerId());
	}
	else
	{
	}
	_HealthChangedEvent.Broadcast();
}

void AGenericGaemPlayerState::OnRep_GameRole()
{
	OnGameRoleUpdate();
}

void AGenericGaemPlayerState::OnGameRoleUpdate()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("Server: Game role updated to %d for player_id: %d"), static_cast<int32>(_AssignedRole), GetPlayerId());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client?: Game role replicated to %d for player_id: %d"), static_cast<int32>(_AssignedRole), GetPlayerId());
	}
	_RoleChangedEvent.Broadcast();
}

void AGenericGaemPlayerState::OnRep_LastTimeLeader()
{
	OnLastTimeLeaderUpdate();
}

void AGenericGaemPlayerState::OnLastTimeLeaderUpdate()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("Server: Last time leader updated to %s for player_id: %d"), *_LastLeaderDateTimeString, GetPlayerId());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client?: Last time leader replicated to %s for player_id: %d"), *_LastLeaderDateTimeString, GetPlayerId());
	}
}

void AGenericGaemPlayerState::OnRep_Money()
{
	OnMoneyUpdate();
}

void AGenericGaemPlayerState::OnMoneyUpdate()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("Server: Money updated to %s for player_id: %d"), *_Money, GetPlayerId());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client?: Money replicated to %s for player_id: %d"), *_Money, GetPlayerId());
	}
	_MoneyChangedEvent.Broadcast();
}