// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaosGameMode.h"
#include "ChaosPlayerState.h"
#include "ChaosGameState.h"
#include "GameCharacter.h"
#include "CharacterChooser.h"

void AChaosGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("Player Joined the Game"));
}

bool AChaosGameMode::CanPurchaseRoleValidation(AChaosGameState* _GameState, APlayerController* PlayerController, const FName& CharacterName)
{
	if (!_GameState || !PlayerController || !CharacterName.IsValid())
	{
		return false;
	}
	const float& Price = _GameState->GetPriceOfRoleCharacter(CharacterName);
	const bool& _PlayerCanPurchaseRole = _GameState->PlayerCanPurchase(PlayerController, Price);
	const ACharacterChooser* _PlayerCharacterChooser = PlayerController->GetPawn<ACharacterChooser>();
	return _PlayerCharacterChooser && _PlayerCanPurchaseRole;
}

bool AChaosGameMode::ServerPurchaseRole_Validate(APlayerController* PlayerController, const FName& CharacterName)
{
	AChaosGameState* _GameState = GetGameState<AChaosGameState>();
	return CanPurchaseRoleValidation(_GameState, PlayerController, CharacterName);
}

void AChaosGameMode::ServerPurchaseRole_Implementation(APlayerController* PlayerController, const FName& CharacterName)
{
	AChaosPlayerState* _PlayerState = PlayerController->GetPlayerState<AChaosPlayerState>();
	_PlayerState->SetMoney(_PlayerState->GetMoney() - GetGameState<AChaosGameState>()->GetPriceOfRoleCharacter(CharacterName));
	_PlayerState->SetRoleName(CharacterName);
	// return _CanPurchase(PlayerController, Price);
}
