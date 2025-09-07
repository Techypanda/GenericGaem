// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string_view>
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ChaosGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AChaosGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void PostLogin(class APlayerController* NewPlayer) override;
	/// <summary>
	/// Handles validation for whether a player can purchase a role, used in ChaosPlayerController::PurchaseRole_Validate
	/// </summary>
	/// <param name="_GameState">The state of the game</param>
	/// <param name="PlayerController">The player controller in question</param>
	/// <param name="CharacterName">The name of the character</param>
	/// <returns>true if success false if not</returns>
	static bool CanPurchaseRoleValidation(class AChaosGameState* _GameState, APlayerController* PlayerController, const FName& CharacterName);
	/// <summary>
	/// Given rolename give a spawnpoint location
	/// </summary>
	/// <param name="RoleName">Role name to lookup spawn point for</param>
	/// <returns>Either a spawnpoint or std::nullopt</returns>
	std::optional<FVector> GetSpawnPointForRole(const FName& RoleName) const;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Spawning", meta = (MustImplement = "RoleSpawnpoint"))
	TSubclassOf<AActor> _RoleSpawnpointClass;
private:
	/// <summary>
	/// Datastructure for storing role spawn points
	/// </summary>
	TMap<FName, FVector> _RoleSpawnpoints;
	/// <summary>
	/// Scans the map
	/// </summary>
	void _ScanMap();
	/// <summary>
	/// Loads the spawn points from map to data structure
	/// </summary>
	void _ScanMapForRoleSpawnpoints();
	/// <summary>
	/// Validates the parameters for MovePlayerToRoleStartingLocation and returns the error message if any occured
	/// </summary>
	/// <param name="PlayerController">Player to move</param>
	/// <param name="RoleName">Role of character</param>
	/// <returns>Either a empty string view or a error</returns>
	const std::string_view _ValidateMovePlayerToRoleStartingLocation(APlayerController* PlayerController, const FName& RoleName);
};
