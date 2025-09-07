// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChaosPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AChaosPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Server RPC that is invoked when the player's role is changed
	/// </summary>
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnPlayerRoleChanged();
protected:
	/// <summary>
	/// Invokes the server to purchase a role for the player, WARNING: If you call this directly without checking if the player can afford it first, they will be disconnected if it fails
	/// </summary>
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void PurchaseRole(const FName& CharacterName);
private:
	/// <summary>
	/// Does a lookup to how much the role costs, updates money and player state role.
	/// </summary>
	void _UpdatePlayerStateRole(const FName& CharacterName);
};
