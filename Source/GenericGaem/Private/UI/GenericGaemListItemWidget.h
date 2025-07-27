// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GenericGaemListItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class UGenericGaemListItemWidget : public UUserWidget
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable)
	void SetShop(class AShop* Shop);
protected:
	class AShop* _Shop;
	UPROPERTY(BlueprintReadOnly)
	float PurchaseProgress;
	UFUNCTION(BlueprintCallable)
	bool SelectRole(FString RoleName);
	UFUNCTION(BlueprintCallable)
	bool BeginPurchaseItem(FString RowName, FString Price);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GenericGaem/Role")
	UDataTable* RoleTable;
};
