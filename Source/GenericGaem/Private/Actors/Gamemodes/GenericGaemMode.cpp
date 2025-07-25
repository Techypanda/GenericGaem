// Fill out your copyright notice in the Description page of Project Settings.
#include "GenericGaemMode.h"
#include <memory>
#include "BaseRole.h"
#include "GenericGaemState.h"
#include "GameFramework/PlayerController.h"
#include "GenericGaemPlayerState.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/GameState.h"
#include "GenericGaemCharacter.h"
#include "GenericGaemRoleSpawnpoint.h"
#include "GenericGaemDeadIslandSpawn.h"
#include "Kismet/GameplayStatics.h"
#include "MeleeWeaponItem.h"
#include "ItemTableRow.h"
#include "BaseRole.h"

static constexpr float _DetermineNextLeaderTimerInterval = 2.0f;
static constexpr std::string_view _StartingMoney = "251";

AGenericGaemMode::AGenericGaemMode() : _DetermineNextLeaderTimerHandler{}, _DebugHandler{}, _RoleSpawnPoints{}
{
}

void AGenericGaemMode::InitializePlayer(APlayerController* NewPlayer)
{
	AGenericGaemPlayerState* PlayerState = Cast<AGenericGaemPlayerState>(NewPlayer->PlayerState);
	if (!PlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState is not of type AGenericGaemPlayerState for player: %s, kicking"), *NewPlayer->GetName());
		FText KickReason = FText::FromString("Failed to retrieve player state, please reconnect");
		GameSession->KickPlayer(NewPlayer, KickReason);
		return;
	}
	PlayerState->SetGameRole(UBaseRole::BaseRoleName);
	PlayerState->SetLastTimeLeader(FDateTime::UtcNow());
	PlayerState->SetMoney(FString(_StartingMoney.data()));
	PlayerState->SetHealth(AGenericGaemPlayerState::MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Assigned PlayerId: %d to player: %s, %s time joined"), PlayerState->GetPlayerId(), *NewPlayer->GetName(), *PlayerState->GetLastTimeLeaderAsDateTime().ToString());
}

void AGenericGaemMode::InitGameState()
{
	Super::InitGameState();
	UE_LOG(LogTemp, Warning, TEXT("AGenericGaemMode::InitGameState called"));
	// We need a leader, poll every 2 seconds till we get one, then it will clear itself
	GetWorld()->GetTimerManager().SetTimer(_DetermineNextLeaderTimerHandler, this, &AGenericGaemMode::DetermineNextLeader, _DetermineNextLeaderTimerInterval, true);
	GetWorld()->GetTimerManager().SetTimer(_DeathTimerHandler, this, &AGenericGaemMode::OnDeathCheck, 1.0f, true);
}

void AGenericGaemMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(_DetermineNextLeaderTimerHandler);
	GetWorld()->GetTimerManager().ClearTimer(_DeathTimerHandler);
}

void AGenericGaemMode::ServerDeath_Implementation(AGenericGaemPlayerState* PlayerState)
{
	UE_LOG(LogTemp, Warning, TEXT("Server Death called on %s"), *GetName());
	const auto& RespawnTime = 10.0f;
	const auto& RespawnTimePoint = FDateTime::Now().ToUnixTimestamp() + static_cast<int64>(RespawnTime);
	const auto& _Player = Cast<AGenericGaemCharacter>(PlayerState->GetPawn());
	PlayerState->InventoryClear();
	auto DeathLocation = _Player->GetActorLocation();
	DeathLocation.Z -= 90.0f;
	const auto Death = GetWorld()->SpawnActor<ADeathObject>(GetGameState<AGenericGaemState>()->GetDeathObject(), DeathLocation, _Player->GetActorRotation(), FActorSpawnParameters{});
	Death->MulticastSetDeath(RespawnTime, PlayerState->GetPlayerName());
	Death->SetDeath(RespawnTime, PlayerState ->GetPlayerName());
	PlayerState->SetTimeTillRespawn(RespawnTime);
	_Player->Death();
	_Player->SetActorLocation(DeadIslandSpawnActor->GetActorLocation());
	_DeadPlayerStates[RespawnTimePoint] = PlayerState;
	UE_LOG(LogTemp, Warning, TEXT("Player %s has died, will respawn at (Unix %lld) time"), *PlayerState->GetPlayerName(), RespawnTimePoint);
}

void AGenericGaemMode::SetPlayerAsLeader(AGenericGaemPlayerState* NewLeader)
{
	NewLeader->SetLastTimeLeader(FDateTime::Now()); // Update the time they were last leader (replicated)
	NewLeader->SetGameRole(UBaseRole::LeaderRoleName); // Set them to leader (replicated)
	Cast<AGenericGaemState>(GameState)->SetLeader(NewLeader->GetPlayerId());
	MovePlayerToSpawnPoint(NewLeader); // Move the player to the spawn point for the leader
}

void AGenericGaemMode::LoadSpawnPoints()
{
	TArray<AActor*> FoundSpawnPoints{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGenericGaemRoleSpawnpoint::StaticClass(), FoundSpawnPoints);
	for (auto& SpawnPointAsActor : FoundSpawnPoints)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found spawn point: %s"), *SpawnPointAsActor->GetName());
		const auto& SpawnPoint = Cast<AGenericGaemRoleSpawnpoint>(SpawnPointAsActor);
		_RoleSpawnPoints[SpawnPoint->RoleToSpawn] = SpawnPoint;
	}
	const auto& DeadIslandActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGenericGaemDeadIslandSpawn::StaticClass());
	DeadIslandSpawnActor = Cast<AGenericGaemDeadIslandSpawn>(DeadIslandActor);
}

void AGenericGaemMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("AGenericGaemMode::PostLogin called for player!: %s"), *NewPlayer->GetName());
	InitializePlayer(NewPlayer);
	// FOR DEBUG
	// after 30 seconds, set the players role to peasant
	// GetWorld()->GetTimerManager().SetTimer(_DebugHandler, this, &AGenericGaemMode::OnDebug, 5.0f, false);
}

// TODO: Maybe multi thread?
void AGenericGaemMode::DetermineNextLeader()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("DetermineNextLeader called on client, ignoring"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Determining next leader..."));
	if (!GameState || GameState->PlayerArray.Num() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameState or PlayerArray is null in DetermineNextLeader, will check again shortly"));
		return;
	}
	auto CharactersWhoHavePickedARole = GameState->PlayerArray.FilterByPredicate([](const APlayerState* PlayerState)
	{
		const auto Pstate = Cast<AGenericGaemPlayerState>(PlayerState);
		if (!Pstate) return false;
		const auto GameRole = Pstate->GetGameRole();
		return GameRole != UBaseRole::BaseRoleName && GameRole != UBaseRole::LeaderRoleName;
		});
	if (CharactersWhoHavePickedARole.Num() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("There is currently no players with a role"));
		return;
	}
	CharactersWhoHavePickedARole.Sort([](const APlayerState& A, const APlayerState& B)
	{
		const auto PstateA = Cast<AGenericGaemPlayerState>(&A);
		const auto PstateB = Cast<AGenericGaemPlayerState>(&B);
		if (!PstateA || !PstateB) return false;
		return PstateA->GetLastTimeLeaderAsDateTime() < PstateB->GetLastTimeLeaderAsDateTime();
	});
	const auto NewLeader = Cast<AGenericGaemPlayerState>(CharactersWhoHavePickedARole[0].Get());
	SetPlayerAsLeader(NewLeader);
	// Leader role is determined, clear timer
	GetWorld()->GetTimerManager().ClearTimer(_DetermineNextLeaderTimerHandler);
	// TODO: We need a timer setup here to check the leader is still alive; if your dead then you lose your leader role
}

void AGenericGaemMode::OnDeathCheck()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDeathCheck called on client, ignoring"));
		return;
	}
	const auto NowUnix = FDateTime::Now().ToUnixTimestamp();
	UE_LOG(LogTemp, Warning, TEXT("OnDeathCheck called at %lld unix (Dead Players Waiting: %d)"), NowUnix, _DeadPlayerStates.size());
	auto Iterator = _DeadPlayerStates.lower_bound(NowUnix);
	for (;;)
	{
		bool bIsBeginning = Iterator == _DeadPlayerStates.begin();
		bool bIsEnd = Iterator == _DeadPlayerStates.end();
		if ((bIsBeginning && bIsEnd) || (!bIsEnd && Iterator->first > NowUnix))
		{
			break;
		}
		if (bIsEnd)
		{
			Iterator--;
			continue;
		}
		const auto& _PlayerState = Iterator->second;
		UE_LOG(LogTemp, Warning, TEXT("Reviving player (%s) with Epoch: %lld"), *_PlayerState->GetPlayerName(), Iterator->first);
		Cast<AGenericGaemCharacter>(_PlayerState->GetPawn())->Revive(); // Reset the pawn
		_PlayerState->Revive(); // Reset The Playerstate
		Iterator = _DeadPlayerStates.erase(Iterator);
		if (bIsBeginning)
		{
			break;
		}
	}
}

void AGenericGaemMode::OnDebug()
{
	UE_LOG(LogTemp, Warning, TEXT("Debug: Setting first player as peasant"));
	const auto NewLeader = Cast<AGenericGaemPlayerState>(GameState->PlayerArray[0].Get());
	//NewLeader->SetMoney(TEXT("1000"));
	//NewLeader->SetGameRole(ERole::Peasant); // Set them to peasant (replicated)
	//NewLeader->SetHealth(19.82942f);
}

void AGenericGaemMode::StartPlay()
{
	Super::StartPlay();
	LoadSpawnPoints();
}

void AGenericGaemMode::MovePlayerToSpawnPoint(AGenericGaemPlayerState* PlayerState)
{
	FScopeLock Lock(&PlayerState->GameRoleLock);
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("MovePlayerToSpawnPoint called on client, ignoring"));
		return;
	}
	 const auto& RoleToSpawn = ERoleHelper::StringToERoleName[PlayerState->GetGameRole()];
	 const auto& SpawnLocation = _RoleSpawnPoints[RoleToSpawn]->GetSpawnLocation();
	 const auto& SpawnRotation = _RoleSpawnPoints[RoleToSpawn]->GetSpawnRotation();
	 UE_LOG(LogTemp, Warning, TEXT("Moving player %s to spawn point for role %d at location %s with rotation %s"),
		*PlayerState->GetPlayerName(), static_cast<int32>(RoleToSpawn), *SpawnLocation.ToString(), *SpawnRotation.ToString());
	const auto& _Controller = Cast<APlayerController>(PlayerState->GetOwner());
	const auto& _Character = Cast<AGenericGaemCharacter>(_Controller->GetCharacter());
	// TODO: Investigate why rotation does not work here
	_Character->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
}
