// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemPlayerState.h"

const FGuid AGenericGaemPlayerState::GetPlayerId() const
{
	return PlayerId;
}

void AGenericGaemPlayerState::SetPlayerId(const FGuid& InPlayerId)
{
	PlayerId = InPlayerId;
}

void AGenericGaemPlayerState::UpdateLastTimeLeader(const std::chrono::system_clock::time_point& InTime)
{
	LastTimeLeader = InTime;
}

const std::chrono::system_clock::time_point& AGenericGaemPlayerState::GetLastTimeLeader() const
{
	return LastTimeLeader;
}
