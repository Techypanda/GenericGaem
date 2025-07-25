// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemPlayerState.h"
#include "GenericGaemState.h"
#include "GenericGaemHUD.h"
#include "GenericGaemMode.h"
#include "Net/UnrealNetwork.h"
#include "GenericGaemCharacter.h"
#include "IItem.h"
#include "RoleTableRow.h"
#include "DataRole.h"
#include "BaseItem.h"
#include "Engine/Engine.h"
#include "Engine/ActorChannel.h"

static constexpr int ActiveItemsCount = 9;

AGenericGaemPlayerState::AGenericGaemPlayerState() : _MoneyChangedEvent{}, _Health{ MaxHealth }, GameRoleLock{}, _EquippedItem{ nullptr }, _ActiveItems{BuildItemsArrayOfSize(ActiveItemsCount)}, _SelectedActiveItem{NoItemSelected}, bIsDead{false}, _TimeTillRespawn{0.0f}
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AGenericGaemPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsDead) {
		_TimeTillRespawn = std::max(_TimeTillRespawn - DeltaTime, 0.0f);
	}
}

void AGenericGaemPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGenericGaemPlayerState, _AssignedRole);
	DOREPLIFETIME(AGenericGaemPlayerState, _LastLeaderDateTimeString);
	DOREPLIFETIME(AGenericGaemPlayerState, bIsInvulnerable);
	DOREPLIFETIME(AGenericGaemPlayerState, _EquippedItem);
	DOREPLIFETIME(AGenericGaemPlayerState, bIsDead);
}

void AGenericGaemPlayerState::Revive()
{
	UE_LOG(LogTemp, Warning, TEXT("Reviving player_id: %d"), GetPlayerId());
	_Health = MaxHealth;
	_EquippedItem = nullptr;
	_ActiveItems = BuildItemsArrayOfSize(ActiveItemsCount);
	_SelectedActiveItem = NoItemSelected;
	_Inventory = TArray<ABaseItem*>{};
	_TimeTillRespawn = 0.0f;
	bIsDead = false;
	SetGameRole(UBaseRole::BaseRoleName);
	ClientRevive();
}

void AGenericGaemPlayerState::SetGameRole(FString NewRole)
{
	FScopeLock Lock(&GameRoleLock);
	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting game role to %s for player_id: %d"), *NewRole, GetPlayerId());
		_AssignedRole = NewRole;
		OnGameRoleUpdate();
	}
}

FString AGenericGaemPlayerState::GetGameRole() const
{
	return _AssignedRole;
}

FColor AGenericGaemPlayerState::GetGameRoleColor() const
{
	return FRoleTableRow::GetDataRole(_AssignedRole, RoleTable)->GetRoleColor();
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

void AGenericGaemPlayerState::ServerPurchaseRole_Implementation(const FString& RoleToPurchase)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("PurchaseRole called on client, but this should only be called on the server!"));
		return;
	}
	const auto LocalRole = FRoleTableRow::GetDataRole(RoleToPurchase, RoleTable);
	const auto Price = FCString::Atoi(*LocalRole->GetRolePrice());
	const auto PlayerMoney = FCString::Atoi(*GetMoney());
	if (PlayerMoney < Price)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough money to purchase role %s. Required: %d, Available: %d"), *RoleToPurchase, Price, PlayerMoney);
		return;
	}
	SetGameRole(RoleToPurchase);
	SetMoney(FString::FromInt(PlayerMoney - Price));
	const auto& PlayerItemsToSpawn = LocalRole->GetStarterItems();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	for (const auto& ItemClass : PlayerItemsToSpawn)
	{
		const auto& _SpawnedItem = GetWorld()->SpawnActor<ABaseItem>(
			ItemClass,
			SpawnParams
		);
		AddToInventory(_SpawnedItem);
	}
	GetWorld()->GetAuthGameMode<AGenericGaemMode>()->MovePlayerToSpawnPoint(this);
	UE_LOG(LogTemp, Warning, TEXT("Role %s purchased successfully for player_id: %d. Remaining money: %s"), *LocalRole->GetRoleName(), GetPlayerId(), *GetMoney());
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
		if (NewHealth <= 0.0f && !bIsDead) {
			bIsDead = true;
			GetWorld()->GetAuthGameMode<AGenericGaemMode>()->ServerDeath(this);
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
	if (ItemToEquip == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("player_id: %d tried to equip None, Equipping nothing"), GetPlayerId());
		_EquippedItem = nullptr;
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

void AGenericGaemPlayerState::ServerEquipItem_Implementation(const TScriptInterface<class IItem>& ItemToEquip)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerEquipItem called for player_id: %d"), GetPlayerId());
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerEquipItem called on client, but this should only be called on the server!"));
		return;
	}
	EquipItem(ItemToEquip);
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
	for (int I = 0; I < _ActiveItems.Num(); I++)
	{
		if (!_ActiveItems[I])
		{
			// Found an empty slot in active items, set it to the new item
			return SetActiveItem(I, Item);
		}
	}
}

void AGenericGaemPlayerState::InventoryClear()
{
	for (int I = 0; I < _Inventory.Num(); I++)
	{
		const auto& ActorToDelete = _Inventory.Pop();
		ActorToDelete->Destroy();
	}
	if (_EquippedItem && _EquippedItem.Get() && !_EquippedItem.Get()->IsActorBeingDestroyed()) {
		_EquippedItem.Get()->Destroy();
	}
	_EquippedItem = nullptr;
	OnInventoryUpdate();
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

ABaseItem* AGenericGaemPlayerState::GetActiveItem(int Index) const
{
	return _ActiveItems[Index];
}

ABaseItem* const& AGenericGaemPlayerState::GetInventoryItem(int Index) const
{
	return _Inventory[Index];
}

void AGenericGaemPlayerState::SetActiveItem(int Index, ABaseItem* Item)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetActiveItem called on client, but this should only be called on the server!"));
		return;
	}
	if (Index < 0 || Index >= _ActiveItems.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid index %d for SetActiveItem, must be between 0 and %d"), Index, _ActiveItems.Num() - 1);
		return;
	}
	_ActiveItems[Index] = Item;
}

void AGenericGaemPlayerState::SetSelectedActiveItem(int Index)
{
	UE_LOG(LogTemp, Warning, TEXT("SetSelectedActiveItem called with Index: %d for player_id: %d"), Index, GetPlayerId());
	if (Index != NoItemSelected && (Index < 0 || Index >= _ActiveItems.Num()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid index %d for SetSelectedActiveItem, must be between 0 and %d"), Index, _ActiveItems.Num() - 1);
		return;
	}
	_SelectedActiveItem = Index;
	// We server RPC here as its called from Widget
	ServerEquipItem(Index != NoItemSelected ? _ActiveItems[Index] : nullptr);
	_SelectedActiveItemChangedEvent.Broadcast();
}

int AGenericGaemPlayerState::GetSelectedActiveItem()
{
	return _SelectedActiveItem;
}

float AGenericGaemPlayerState::GetTimeTillRespawn() const
{
	return _TimeTillRespawn;
}

void AGenericGaemPlayerState::SetTimeTillRespawn(float InTime)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		// doesn't really matter since its UI, but may aswell
		UE_LOG(LogTemp, Warning, TEXT("SetTimeTillRespawn called on client, but this should only be called on the server!"));
		return;
	}
	_TimeTillRespawn = InTime;
}

void AGenericGaemPlayerState::ClientRevive_Implementation ()
{
	OnPlayerRevive().Broadcast();
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
	if (_Health <= 0.0f)
	{
		OnPlayerDeath().Broadcast();
	}
	_HealthChangedEvent.Broadcast();
}

void AGenericGaemPlayerState::OnRep_GameRole()
{
	OnGameRoleUpdate();
}

void AGenericGaemPlayerState::OnGameRoleUpdate()
{
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

TArray<ABaseItem*> AGenericGaemPlayerState::BuildItemsArrayOfSize(int Size)
{
	TArray<ABaseItem*> _ItemsArr;
	for (int I = 0; I < Size; I++)
	{
		_ItemsArr.Add(nullptr); // we want to make sure theres always 9 items
	}
	return _ItemsArr;
}
