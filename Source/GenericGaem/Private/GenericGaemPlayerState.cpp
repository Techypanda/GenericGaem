// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

AGenericGaemPlayerState::AGenericGaemPlayerState()
{
	bReplicates = true;
}

void AGenericGaemPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGenericGaemPlayerState, _AssignedRole);
	DOREPLIFETIME(AGenericGaemPlayerState, _LastLeaderDateTimeString);
}

void AGenericGaemPlayerState::SetGameRole(ERole NewRole)
{
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

