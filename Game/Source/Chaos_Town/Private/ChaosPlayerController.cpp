// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaosPlayerController.h"
#include "GameCharacter.h"
#include "ChaosGameMode.h"
#include "ChaosGameState.h"

bool AChaosPlayerController::PurchaseRole_Validate(const FName& CharacterName)
{
	AChaosGameState* _GameState = GetWorld()->GetGameState<AChaosGameState>();
	return AChaosGameMode::CanPurchaseRoleValidation(_GameState, this, CharacterName);
}

void AChaosPlayerController::PurchaseRole_Implementation(const FName& CharacterName)
{
	GetWorld()->GetAuthGameMode<AChaosGameMode>()->ServerPurchaseRole(this, CharacterName);
}
