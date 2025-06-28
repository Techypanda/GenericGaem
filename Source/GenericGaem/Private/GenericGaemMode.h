// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
protected:
	FTimerHandle _DetermineNextLeaderTimerHandler;
	FTimerHandle _DebugHandler;
private:
	void SetPlayerAsLeader(class AGenericGaemPlayerState* NewLeader);
};
