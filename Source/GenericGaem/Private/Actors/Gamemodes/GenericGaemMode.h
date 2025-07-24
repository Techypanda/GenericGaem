// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <map>
#include <unordered_map>
#include "ERole.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/TimerHandle.h"
#include "ItemTableRow.h"
#include "GenericGaemMode.generated.h"

/**
 * 
 */
UCLASS()
class AGenericGaemMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AGenericGaemMode();
	void InitializePlayer(class APlayerController* NewPlayer);
	void PostLogin(APlayerController* NewPlayer) override;
	void InitGameState() override;
	void DetermineNextLeader();
	void OnDeathCheck();
	void OnDebug();
	void StartPlay() override;
	void MovePlayerToSpawnPoint(class AGenericGaemPlayerState* PlayerState);
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION(Server, Reliable)
	void ServerDeath(class AGenericGaemPlayerState* PlayerState);
protected:
	FTimerHandle _DetermineNextLeaderTimerHandler;
	FTimerHandle _DeathTimerHandler;
	FTimerHandle _DebugHandler;
private:
	class AGenericGaemDeadIslandSpawn* DeadIslandSpawnActor;
	// Need a RB Tree to get all dead players at a certain time
	std::map<int64, class AGenericGaemPlayerState*> _DeadPlayerStates;
	std::unordered_map<ERole, class AGenericGaemRoleSpawnpoint*> _RoleSpawnPoints;
	void SetPlayerAsLeader(class AGenericGaemPlayerState* NewLeader);
	void LoadSpawnPoints();
};
