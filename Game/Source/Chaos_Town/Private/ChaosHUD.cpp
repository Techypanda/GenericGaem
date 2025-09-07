// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaosHUD.h"
#include "Blueprint/UserWidget.h"

void AChaosHUD::BeginPlay()
{
	Super::BeginPlay();
	_CreateWidgets();
	_ShowRoleSelectionUI(true);
}

void AChaosHUD::OnRolePurchase()
{
	_ShowRoleSelectionUI(false);
}

void AChaosHUD::_CreateWidgets()
{
	_RoleSelectionUI = CreateWidget<UUserWidget>(GetWorld(), _RoleSelectionUIClass);
}

void AChaosHUD::_ShowRoleSelectionUI(bool bShow)
{
	GetOwningPlayerController()->bShowMouseCursor = bShow;
	if (bShow)
	{
		GetOwningPlayerController()->SetInputMode(FInputModeUIOnly());
		_RoleSelectionUI->AddToViewport();
	}
	else
	{
		GetOwningPlayerController()->SetInputMode(FInputModeGameOnly());
		_RoleSelectionUI->RemoveFromViewport();
	}
}
