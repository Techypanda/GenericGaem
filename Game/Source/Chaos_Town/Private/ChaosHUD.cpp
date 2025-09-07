// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaosHUD.h"
#include "Blueprint/UserWidget.h"

void AChaosHUD::BeginPlay()
{
	Super::BeginPlay();
	_ShowRoleSelectionUI();
}

void AChaosHUD::_ShowRoleSelectionUI()
{
	_RoleSelectionUI = CreateWidget<UUserWidget>(GetWorld(), _RoleSelectionUIClass);
	GetOwningPlayerController()->bShowMouseCursor = true;
	GetOwningPlayerController()->SetInputMode(FInputModeUIOnly());
	_RoleSelectionUI->AddToViewport();
}
