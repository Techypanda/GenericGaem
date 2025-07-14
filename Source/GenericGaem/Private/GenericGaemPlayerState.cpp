// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemPlayerState.h"
#include "GenericGaemHUD.h"
#include "GenericGaemMode.h"
#include "Net/UnrealNetwork.h"
#include "GenericGaemCharacter.h"
#include "Engine/Engine.h"

AGenericGaemPlayerState::AGenericGaemPlayerState() : _MoneyChangedEvent{}, _Health{ MaxHealth }, GameRoleLock{}
{
	bReplicates = true;
}

void AGenericGaemPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGenericGaemPlayerState, _AssignedRole);
	DOREPLIFETIME(AGenericGaemPlayerState, _LastLeaderDateTimeString);
	DOREPLIFETIME(AGenericGaemPlayerState, bIsInvulnerable);
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
	if (GetLocalRole() == ROLE_Authority && !bIsInvulnerable)
	{
		NewHealth = std::clamp(NewHealth, 0.0f, MaxHealth);
		if (NewHealth <= 0.0f) {
			const auto _Player = GetPawn<AGenericGaemCharacter>();
			_Player->ServerDeath();
		}
		_Health = NewHealth;
		OnHealthUpdate();
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