// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ChaosGameState.generated.h"

/**
 * 
 */
UCLASS()
class AChaosGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<TSubclassOf<AGameCharacter>> GetRoleCharacterClasses();
	/// <summary>
	/// Checks if a role exists in the role data table
	/// </summary>
	/// <param name="RoleName">Role to check</param>
	/// <returns>true or false</returns>
	bool RoleExists(const FName& RoleName) const;
	/// <summary>
	/// Looks up the character class for a given name
	/// </summary>
	/// <param name="RoleName">name of role</param>
	/// <returns>Character Class</returns>
	TSubclassOf<AGameCharacter> GetRoleForName(const FName& RoleName) const;
	/// <summary>
	/// Returns the price for a character name
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable)
	float GetPriceOfRoleCharacter(FName RoleName);
	void BeginPlay() override;
	/// <summary>
	/// Returns true if the player can afford the purchase
	/// </summary>
	/// <param name="PlayerController">The controller for player</param>
	/// <param name="Price">The price to check</param>
	/// <returns>True/False on if the purchase will succeed</returns>
	UFUNCTION(BlueprintCallable)
	bool PlayerCanPurchase(class APlayerController* PlayerController, float Price);
protected:
	UPROPERTY(EditDefaultsOnly)
	class UDataTable* _RoleTable;

private:
	/// <summary>
	/// Avoid looking up multiple times, once read once save it to memory in this var
	/// </summary>
	TArray<TSubclassOf<AGameCharacter>> _CachedCharacterClasses;
	/// <summary>
	/// TODO: Combine this and above
	/// </summary>
	TMap<FName, TSubclassOf<AGameCharacter>> _RoleNameToCharacterClassMap;
	/// <summary>
	/// Avoid looking up multiple times, once read once save it to memory in this var
	/// </summary>
	TMap<FName, float> _CachedCharacterPrices;
	/// <summary>
	/// Parses the role data table and caches the data for faster access later
	/// </summary>
	void _LoadAndCacheRoleData();
	/// <summary>
	/// Caches the role data character classes from the role data table
	/// </summary>
	/// <returns>The cached roledata</returns>
	void _CacheRoleDataCharacterClasses(TArray<struct FRoleData*>& RoleDataArr);
	/// <summary>
	/// Caches the role data prices from the role data table
	/// </summary>
	void _CacheRoleDataPrices(TArray<FName>& RoleDataArr);
	/// <summary>
	/// Check if lengths of caches are zero, if so load and cache role data
	/// </summary>
	void _CacheRoleDataPricesIfNeeded();
};
