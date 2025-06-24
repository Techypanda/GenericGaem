// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <chrono>
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GenericGaemPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class AGenericGaemPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	const FGuid GetPlayerId() const;
	void SetPlayerId(const FGuid& InPlayerId);
	void UpdateLastTimeLeader(const std::chrono::system_clock::time_point& InTime);
	const std::chrono::system_clock::time_point& GetLastTimeLeader() const;
private:
	FGuid PlayerId;
	std::chrono::system_clock::time_point LastTimeLeader;
};
