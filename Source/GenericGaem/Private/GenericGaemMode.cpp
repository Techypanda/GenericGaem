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
#include "Kismet/GameplayStatics.h"
#include "MeleeWeaponItem.h"

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
	PlayerState->SetGameRole(ERole::None);
	PlayerState->SetLastTimeLeader(FDateTime::UtcNow());
	PlayerState->SetMoney(FString(_StartingMoney.data()));
	PlayerState->SetHealth(AGenericGaemPlayerState::MaxHealth);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerState;
	const auto& _StarterWeaponSpawned = GetWorld()->SpawnActor<AMeleeWeaponItem>(
		StarterWeapon,
		SpawnParams
	);
	PlayerState->AddToInventory(_StarterWeaponSpawned);
	UE_LOG(LogTemp, Warning, TEXT("Assigned PlayerId: %d to player: %s, %s time joined"), PlayerState->GetPlayerId(), *NewPlayer->GetName(), *PlayerState->GetLastTimeLeaderAsDateTime().ToString());
}

void AGenericGaemMode::InitGameState()
{
	Super::InitGameState();
	UE_LOG(LogTemp, Warning, TEXT("AGenericGaemMode::InitGameState called"));
	// We need a leader, poll every 2 seconds till we get one, then it will clear itself
	GetWorld()->GetTimerManager().SetTimer(_DetermineNextLeaderTimerHandler, this, &AGenericGaemMode::DetermineNextLeader, _DetermineNextLeaderTimerInterval, true);
}

void AGenericGaemMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(_DetermineNextLeaderTimerHandler);
}

void AGenericGaemMode::SetPlayerAsLeader(AGenericGaemPlayerState* NewLeader)
{
	NewLeader->SetLastTimeLeader(FDateTime::Now()); // Update the time they were last leader (replicated)
	NewLeader->SetGameRole(ERole::Leader); // Set them to leader (replicated)
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
	for (int I = 0; I <= static_cast<int>(ERole::Leader); I++)
	{
		if (!_RoleSpawnPoints[static_cast<ERole>(I)])
		{
			// TODO: Loop and try again, this should never happen though
		}
	}
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
		return GameRole != ERole::None && GameRole != ERole::Leader;
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
	const auto& RoleToSpawn = PlayerState->GetGameRole();
	const auto& SpawnLocation = _RoleSpawnPoints[RoleToSpawn]->GetSpawnLocation();
	const auto& SpawnRotation = _RoleSpawnPoints[RoleToSpawn]->GetSpawnRotation();
	UE_LOG(LogTemp, Warning, TEXT("Moving player %s to spawn point for role %d at location %s with rotation %s"),
		*PlayerState->GetPlayerName(), static_cast<int32>(RoleToSpawn), *SpawnLocation.ToString(), *SpawnRotation.ToString());
	const auto& _Controller = Cast<APlayerController>(PlayerState->GetOwner());
	const auto& _Character = Cast<AGenericGaemCharacter>(_Controller->GetCharacter());
	// TODO: Investigate why rotation does not work here
	_Character->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
}
