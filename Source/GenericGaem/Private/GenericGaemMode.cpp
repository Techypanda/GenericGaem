// Fill out your copyright notice in the Description page of Project Settings.
#include "GenericGaemMode.h"
#include "GameFramework/PlayerController.h"
#include "GenericGaemPlayerState.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/GameState.h"

AGenericGaemMode::AGenericGaemMode()
{
}

void AGenericGaemMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("AGenericGaemMode::PostLogin called for player!: %s"), *NewPlayer->GetName());
	AGenericGaemPlayerState* PlayerState = Cast<AGenericGaemPlayerState>(NewPlayer->PlayerState);
	if (!PlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState is not of type AGenericGaemPlayerState for player: %s, kicking"), *NewPlayer->GetName());
		FText KickReason = FText::FromString("Failed to retrieve player state, please reconnect");
		GameSession->KickPlayer(NewPlayer, KickReason);
		return;
	}
	const auto TimeJoined = std::chrono::system_clock::now();
	const auto PlayerId = FGuid::NewGuid();
	PlayerState->SetPlayerId(PlayerId);
	PlayerState->UpdateLastTimeLeader(TimeJoined);
	UE_LOG(LogTemp, Warning, TEXT("Assigned PlayerId: %s to player: %s, %lld time joined"), *PlayerId.ToString(), *NewPlayer->GetName(), TimeJoined.time_since_epoch().count());
}