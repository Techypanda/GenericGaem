// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemWidget.h"
#include "GenericGaemPlayerState.h"
#include "GameFramework/PlayerController.h"

void UGenericGaemWidget::InitializeWidget(UWorld* World)
{
	UE_LOG(LogTemp, Warning, TEXT("UGenericGaemWidget::InitializeWidget called"));
	const auto PlayerState = GetOwningPlayerState<AGenericGaemPlayerState>();
	if (!PlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGenericGaemWidget::InitializeWidget: PlayerState is null, going to sleep and restart in a timer"));
		FTimerDelegate InitWidgetDelegate = FTimerDelegate::CreateUObject(this, &UGenericGaemWidget::InitializeWidget, World);
		World->GetTimerManager().SetTimer(_HackyTimerHandle, InitWidgetDelegate, 0.2f, false);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("UGenericGaemWidget::InitializeWidget: PlayerState is not null, yippee"));
	PlayerState->OnMoneyChanged().AddUObject(this, &UGenericGaemWidget::OnMoneyChanged);
	PlayerState->OnRoleChanged().AddUObject(this, &UGenericGaemWidget::OnRoleChanged);
}

void UGenericGaemWidget::PostLoad()
{
	Super::PostLoad();
}

void UGenericGaemWidget::OnMoneyChanged_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Money changed in GenericGaemWidget"));
}

void UGenericGaemWidget::OnRoleChanged_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Role changed in GenericGaemWidget"));
}