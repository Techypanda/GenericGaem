// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaosPlayerController.h"
#include "ChaosPlayerState.h"
#include "GameCharacter.h"
#include "CharacterChooser.h"
#include "ChaosGameMode.h"
#include "ChaosGameState.h"

bool AChaosPlayerController::PurchaseRole_Validate(const FName& CharacterName)
{
	AChaosGameState* _GameState = GetWorld()->GetGameState<AChaosGameState>();
	return AChaosGameMode::CanPurchaseRoleValidation(_GameState, this, CharacterName);
}

void AChaosPlayerController::PurchaseRole_Implementation(const FName& CharacterName)
{
	_UpdatePlayerStateRole(CharacterName);
}

bool AChaosPlayerController::ServerOnPlayerRoleChanged_Validate()
{
	return GetWorld()->GetAuthGameMode<AChaosGameMode>()->GetSpawnPointForRole(GetPlayerState<AChaosPlayerState>()->GetRoleName()).has_value();
}

void AChaosPlayerController::ServerOnPlayerRoleChanged_Implementation()
{
	// Delete And Unpossess old pawn
	APawn* _CharacterChooser = GetPawn();
	if (_CharacterChooser)
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying character chooser"));
		_CharacterChooser->Destroy();
	}
	UnPossess();
	// SPAWN NEW PAWN
	const FName& RoleName = GetPlayerState<AChaosPlayerState>()->GetRoleName();
	const FVector& _SpawnLocation = GetWorld()->GetAuthGameMode<AChaosGameMode>()->GetSpawnPointForRole(RoleName).value();
	const auto& _ActorClassToSpawn = GetWorld()->GetGameState<AChaosGameState>()->GetRoleForName(RoleName);
	FActorSpawnParameters _SpawnParams;
	const auto& _SpawnedActor = GetWorld()->SpawnActor<APawn>(_ActorClassToSpawn, _SpawnLocation, FRotator::ZeroRotator, _SpawnParams);
	Possess(_SpawnedActor);
	// _HandleMovePlayerToRoleStartingLocation();
}

void AChaosPlayerController::_UpdatePlayerStateRole(const FName& CharacterName)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Error, TEXT("UpdatePlayerStateRole can only be called on the server"));
		return;
	}
	AChaosPlayerState* _PlayerState = GetPlayerState<AChaosPlayerState>();
	if (!_PlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerState is null, cannot update role"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Player %s is purchasing role %s"), *_PlayerState->GetPlayerName(), *CharacterName.ToString());
	_PlayerState->SetRoleName(CharacterName);
	_PlayerState->SetMoney(_PlayerState->GetMoney() - GetWorld()->GetGameState<AChaosGameState>()->GetPriceOfRoleCharacter(CharacterName));
}
