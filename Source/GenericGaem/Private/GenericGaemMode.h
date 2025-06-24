// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GenericGaemMode.generated.h"

/**
 * 
 */
UCLASS()
class AGenericGaemMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	void PostLogin(APlayerController* NewPlayer) override;
};
