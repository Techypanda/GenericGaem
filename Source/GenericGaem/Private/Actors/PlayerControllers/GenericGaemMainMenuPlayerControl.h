// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericGaemMainMenuPlayerControl.generated.h"

/**
 * 
 */
UCLASS()
class AGenericGaemMainMenuPlayerControl : public APlayerController
{
	GENERATED_BODY()
public:
	void BeginPlay() override;
};
