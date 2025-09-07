// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	virtual void PostLogin(class APlayerController* NewPlayer) override;
	/// <summary>
	/// Purchase a role for the player if they have enough money
	/// </summary>
	/// <param name="PlayerController">The player's controller that wishes to purchase role</param>
	/// <returns>True/False on if the purchase succeeded</returns>
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ServerPurchaseRole(class APlayerController* PlayerController, const FName& CharacterName);
	static bool CanPurchaseRoleValidation(class AChaosGameState* _GameState, APlayerController* PlayerController, const FName& CharacterName);
};
