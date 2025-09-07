// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ChaosHUD.generated.h"

/**
 * 
 */
UCLASS()
class AChaosHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	/// <summary>
	/// Called once role has been purchased
	/// </summary>
	void OnRolePurchase();
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> _RoleSelectionUIClass;
private:
	class UUserWidget* _RoleSelectionUI;
	/// <summary>
	/// Handles creating the widgets used in the HUD
	/// </summary>
	void _CreateWidgets();
	/// <summary>
	/// Handling toggling and off the role selection UI
	/// </summary>
	void _ShowRoleSelectionUI(bool bShow);
};
