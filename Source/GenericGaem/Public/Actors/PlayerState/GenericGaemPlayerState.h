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
	static constexpr int NoItemSelected = -1;
	static constexpr int MaxInventorySize = 32;
	// make sure that it finishes assigning a role before it assigns a new one
	mutable FCriticalSection GameRoleLock;
	static constexpr float MaxHealth = 100.0f;
	AGenericGaemPlayerState();
	void Tick(float DeltaTime) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void Revive();
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
	DECLARE_EVENT(FLayerViewModel, FSelectedActiveItemChanged)
	FSelectedActiveItemChanged& OnSelectedActiveItemChanged() { return _SelectedActiveItemChangedEvent; }
	DECLARE_EVENT(FLayerViewModel, FMoneyChangedEvent)
	FMoneyChangedEvent& OnMoneyChanged() { return _MoneyChangedEvent; }
	DECLARE_EVENT(FLayerViewModel, FRoleChangedEvent)
	FRoleChangedEvent& OnRoleChanged() { return _RoleChangedEvent; }
	DECLARE_EVENT(FLayerViewModel, FRolePurchased)
	FRolePurchased& OnRolePurchased() { return _RolePurchasedEvent; }
	DECLARE_EVENT(FLayerViewModel, FHealthChangedEvent)
	FHealthChangedEvent& OnHealthChanged() { return _HealthChangedEvent; }
	DECLARE_EVENT(FLayerViewModel, FInventoryChangedEvent)
	FHealthChangedEvent& OnInventoryChanged() { return _HealthChangedEvent; }
	DECLARE_EVENT(FLayerViewModel, FPlayerDeathEvent)
	FPlayerDeathEvent& OnPlayerDeath() { return _PlayerDeathEvent; }
	DECLARE_EVENT(FLayerViewModel, FPlayerReviveEvent)
	FPlayerReviveEvent& OnPlayerRevive() { return _PlayerReviveEvent; }
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
	UFUNCTION(Server, Unreliable)
	void ServerEquipItem(const TScriptInterface<class IItem>& ItemToEquip);
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Inventory")
	void AddToInventory(class ABaseItem* Item);
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Inventory")
	void InventoryClear();
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Inventory")
	bool HasItemInInventory(TScriptInterface<class IItem> ItemToEquip);
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Inventory")
	class ABaseItem* GetActiveItem(int Index) const;
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Inventory")
	class ABaseItem* const& GetInventoryItem(int Index) const;
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Inventory")
	void SetActiveItem(int Index, class ABaseItem* Item);
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Inventory")
	void SetSelectedActiveItem(int Index);
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Inventory")
	int GetSelectedActiveItem();
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Death")
	float GetTimeTillRespawn() const;
	UFUNCTION(BlueprintCallable, Category = "GenericGaem/Death")
	void SetTimeTillRespawn(float InTime);
protected:
	UPROPERTY(Replicated)
	float _TimeTillRespawn;
	UPROPERTY(Replicated)
	bool bIsDead;
	// TStaticArray would work great here! - but... UE5 cant do UPROPERTY with it - https://stackoverflow.com/questions/77759308/unreal-engine-5-cant-find-tstaticarray
	UPROPERTY(ReplicatedUsing = OnRep_Inventory)
	TArray<class ABaseItem*> _Inventory;
	UPROPERTY(Replicated)
	TArray<class ABaseItem*> _ActiveItems;
	UPROPERTY(Replicated)
	int _SelectedActiveItem;
	UFUNCTION(Client, Reliable)
	void ClientRevive();
	UFUNCTION()
	void OnRep_Inventory();
	void OnInventoryUpdate();
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
	FInventoryChangedEvent _InventoryChangedEvent;
	FSelectedActiveItemChanged _SelectedActiveItemChangedEvent;
	FPlayerDeathEvent _PlayerDeathEvent;
	FPlayerReviveEvent _PlayerReviveEvent;
	// Unreal Engine seems to suggest storing a UObject pointer for interfaces... https://dev.epicgames.com/documentation/en-us/unreal-engine/interfaces-in-unreal-engine#safelystoreobjectandinterfacepointers
	UPROPERTY(Replicated)
	TObjectPtr<class ABaseItem> _EquippedItem;
private:
	static TArray<class ABaseItem*> BuildItemsArrayOfSize(int Size);
};
