// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <unordered_map>
#include "ERole.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/TimerHandle.h"
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
	void PostLogin(APlayerController* NewPlayer) override;
	void InitGameState() override;
	void DetermineNextLeader();
	void OnDebug();
	void StartPlay() override;
	void MovePlayerToSpawnPoint(class AGenericGaemPlayerState* PlayerState);
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
protected:
	FTimerHandle _DetermineNextLeaderTimerHandler;
	FTimerHandle _DebugHandler;
private:
	std::unordered_map<ERole, class AGenericGaemRoleSpawnpoint*> _RoleSpawnPoints;
	void SetPlayerAsLeader(class AGenericGaemPlayerState* NewLeader);
	void LoadSpawnPoints();
};
