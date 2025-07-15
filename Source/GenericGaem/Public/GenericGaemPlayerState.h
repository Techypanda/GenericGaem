// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <chrono>
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ERole.h"
#include "Math/Color.h"
#include "GenericGaemPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class AGenericGaemPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	// make sure that it finishes assigning a role before it assigns a new one
	mutable FCriticalSection GameRoleLock;
	static constexpr float MaxHealth = 100.0f;
	AGenericGaemPlayerState();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/GameRole")
	void SetGameRole(ERole NewRole);
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/GameRole")
	ERole GetGameRole() const;
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/GameRole")
	FString GetGameRoleAsString() const;
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/GameRole")
	FColor GetGameRoleColor() const;
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Stats")
	void SetLastTimeLeader(FDateTime NewDateTime);
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Stats")
	FDateTime GetLastTimeLeaderAsDateTime() const;
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Stats")
	void SetMoney(FString NewMoney);
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Stats")
	FString GetMoney() const;
	DECLARE_EVENT(FLayerViewModel, FMoneyChangedEvent)
	FMoneyChangedEvent& OnMoneyChanged() { return _MoneyChangedEvent; }
	DECLARE_EVENT(FLayerViewModel, FRoleChangedEvent)
	FRoleChangedEvent& OnRoleChanged() { return _RoleChangedEvent; }
	DECLARE_EVENT(FLayerViewModel, FRolePurchased)
	FRolePurchased& OnRolePurchased() { return _RolePurchasedEvent; }
	DECLARE_EVENT(FLayerViewModel, FHealthChangedEvent)
	FHealthChangedEvent& OnHealthChanged() { return _HealthChangedEvent; }
	UFUNCTION(Server, Reliable)
	void ServerPurchaseRole(ERole RoleToPurchase);
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Stats")
	void SetHealth(float NewHealth);
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Stats")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Stats")
	void SetInvulnerability(bool bInInvulnerability);
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Stats")
	bool GetInvulnerability() const;
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Inventory")
	const TScriptInterface<class IItem> GetEquippedItem() const;
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Inventory")
	void EquipItem(TScriptInterface<class IItem> ItemToEquip);
protected:
	UPROPERTY(ReplicatedUsing = OnRep_Invulnerability)
	bool bIsInvulnerable;
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	float _Health;
	UFUNCTION()
	void OnRep_Health();
	UFUNCTION()
	void OnRep_Invulnerability();
	void OnInvulnerabilityUpdate();
	void OnHealthUpdate();
	UPROPERTY(ReplicatedUsing = OnRep_GameRole)
	ERole _AssignedRole;
	UFUNCTION()
	void OnRep_GameRole();
	void OnGameRoleUpdate();
	UPROPERTY(ReplicatedUsing = OnRep_LastTimeLeader)
	FString _LastLeaderDateTimeString;
	UFUNCTION()
	void OnRep_LastTimeLeader();
	void OnLastTimeLeaderUpdate();
	// Unreal Engine doesn't support uint64, so I'm just going to use a string as sad
	// as that is.
	UPROPERTY(ReplicatedUsing = OnRep_Money)
	FString _Money;
	UFUNCTION()
	void OnRep_Money();
	UFUNCTION()
	void OnMoneyUpdate();
	FMoneyChangedEvent _MoneyChangedEvent;
	FRoleChangedEvent _RoleChangedEvent;
	FRolePurchased _RolePurchasedEvent;
	FHealthChangedEvent _HealthChangedEvent;
	// Unreal Engine seems to suggest storing a UObject pointer for interfaces... https://dev.epicgames.com/documentation/en-us/unreal-engine/interfaces-in-unreal-engine#safelystoreobjectandinterfacepointers
	UObject* _EquippedItem;
};
