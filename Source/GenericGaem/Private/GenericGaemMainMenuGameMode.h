// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GenericGaemMainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AGenericGaemMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidget;

	virtual void BeginPlay() override;
};
