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

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> _RoleSelectionUIClass;

private:
	class UUserWidget* _RoleSelectionUI;
	/// <summary>
	/// Handles creating the Role Selection UI and adding it to the viewport aswell as
	/// enabling the mouse cursor and setting the input mode to UI only.
	/// </summary>
	void _ShowRoleSelectionUI();
};
