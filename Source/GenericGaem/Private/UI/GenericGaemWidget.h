// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GenericGaemListItemWidget.h"
#include "GenericGaemWidget.generated.h"

/**
 * 
 */
UCLASS()
class UGenericGaemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void InitializeWidget(class UWorld* World);
protected:
	void PostLoad() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GenericGaem")
	void OnMoneyChanged();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GenericGaem")
	void OnRoleChanged();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GenericGaem")
	void OnRolePurchased();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GenericGaem")
	void OnHealthChanged();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GenericGaem")
	void OnEscapeMenu();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GenericGaem")
	void OnActiveItemSelectedChanged();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GenericGaem")
	void OnPlayerDeath();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GenericGaem")
	void OnPlayerRevive();
	UFUNCTION(BlueprintCallable, Category = "GenericGaem")
	TArray<struct FRoleListItem> GetRoleListItems() const;
	FTimerHandle _HackyTimerHandle;
};
