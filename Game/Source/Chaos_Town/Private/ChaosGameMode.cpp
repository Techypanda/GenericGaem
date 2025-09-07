// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaosGameMode.h"
#include "ChaosPlayerState.h"
#include "ChaosGameState.h"
#include "GameCharacter.h"
#include "CharacterChooser.h"
#include "RoleSpawnPoint.h"
#include "Kismet/GameplayStatics.h"

void AChaosGameMode::BeginPlay()
{
	Super::BeginPlay();
	_ScanMap();
}

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

std::optional<FVector> AChaosGameMode::GetSpawnPointForRole(const FName& RoleName) const
{
	if (_RoleSpawnpoints.Contains(RoleName))
	{
		return _RoleSpawnpoints[RoleName];
	}
	return std::nullopt;
}

//void AChaosGameMode::MovePlayerToRoleStartingLocation(APlayerController* PlayerController, const FName& RoleName)
//{
//	return;
//	const std::string_view& _Error = _ValidateMovePlayerToRoleStartingLocation(PlayerController, RoleName);
//	if (!_Error.empty())
//	{
//		UE_LOG(LogTemp, Error, TEXT("MovePlayerToRoleStartingLocation: Failed Validation: %hs"), _Error.data());
//		return;
//	}
//	const FVector& SpawnLocation = _RoleSpawnpoints[RoleName];
//	UE_LOG(LogTemp, Warning, TEXT("Moving player to role %s at location %s"), *RoleName.ToString(), *SpawnLocation.ToString());
//	PlayerController->GetPawn()->SetActorLocation(SpawnLocation);
//}

void AChaosGameMode::_ScanMap()
{
	UE_LOG(LogTemp, Warning, TEXT("Scanning map begun"));
	_ScanMapForRoleSpawnpoints();
	UE_LOG(LogTemp, Warning, TEXT("Finished scanning map after ..."));
}

void AChaosGameMode::_ScanMapForRoleSpawnpoints()
{
	TArray<AActor*> _FoundActors;
	UE_LOG(LogTemp, Warning, TEXT("Scanning map for role spawnpoints"));
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), _RoleSpawnpointClass, _FoundActors);
	for (AActor* _Actor : _FoundActors)
	{
		if (_Actor->Implements<URoleSpawnPoint>())
		{
			const FName RoleName = IRoleSpawnPoint::Execute_GetRoleName(_Actor);
			const FVector SpawnLocation = IRoleSpawnPoint::Execute_GetSpawnLocation(_Actor);
			_RoleSpawnpoints.Add(RoleName, SpawnLocation);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Finished scanning map for role spawnpoints after ..."));
}

const std::string_view AChaosGameMode::_ValidateMovePlayerToRoleStartingLocation(APlayerController* PlayerController, const FName& RoleName)
{
	const AChaosGameState* _GameState = GetWorld()->GetGameState<AChaosGameState>();
	if (!PlayerController)
	{
		return "Player Controller is null";
	}
	if (!RoleName.IsValid() || !_GameState->RoleExists(RoleName))
	{
		return "RoleName is not valid";
	}
	if (!_RoleSpawnpoints.Contains(RoleName))
	{
		return "No spawn point found for role";
	}
	if (GetLocalRole() != ROLE_Authority)
	{
		return "Only Server can call this method";
	}
	return std::string_view{};
}
