// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ChaosPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class AChaosPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AChaosPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/// <summary>
	/// Returns player's money
	/// </summary>
	/// <returns>Player's Money</returns>
	UFUNCTION(BlueprintCallable)
	float GetMoney() const;
	/// <summary>
	/// Sets the player's money, can only be called on the server
	/// </summary>
	/// <param name="NewMoney">The money to set to</param>
	void SetMoney(const float& NewMoney);
	/// <summary>
	/// Returns player's role
	/// </summary>
	/// <returns>Player's Role</returns>
	FName GetRoleName() const;
	/// <summary>
	/// Sets the player's role can only be called on the server
	/// </summary>
	/// <param name="NewRoleName">The rolename to set to</param>
	void SetRoleName(const FName& NewRoleName);
protected:
	/// <summary>
	/// The amount of money the player has (Replicated)
	/// </summary>
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_Money)
	float _Money;
	/// <summary>
	/// The role name of the player (Replicated)
	/// </summary>
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_Role)
	FName _RoleName;
	/// <summary>
	/// Notifier for when money is replicated
	/// </summary>
	UFUNCTION()
	void OnRep_Money();
	/// <summary>
	/// Called when money is changed, can be used for UI updates
	/// </summary>
	void OnMoneyChanged();
	/// <summary>
	/// Notifier for when role is replicated
	/// </summary>
	UFUNCTION()
	void OnRep_Role();
	/// <summary>
	/// Called when the role is changed, can be used for UI updates
	/// </summary>
	void OnRoleChanged();
};
