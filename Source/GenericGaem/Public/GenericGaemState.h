// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GenericGaemState.generated.h"

/**
 * 
 */
UCLASS()
class GENERICGAEM_API AGenericGaemState : public AGameStateBase
{
	GENERATED_BODY()
	
 public:
	void PreInitializeComponents() override;
};
