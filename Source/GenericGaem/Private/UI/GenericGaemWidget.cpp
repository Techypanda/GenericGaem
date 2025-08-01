// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemWidget.h"
#include "GenericGaemPlayerState.h"
#include "GenericGaemCharacter.h"
#include "GenericGaemStructures.h"
#include "RoleTableRow.h"
#include "DataRole.h"
#include "Shop.h"
#include "GameFramework/PlayerController.h"

void UGenericGaemWidget::InitializeWidget(UWorld* World)
{
	UE_LOG(LogTemp, Warning, TEXT("UGenericGaemWidget::InitializeWidget called"));
	const auto PlayerState = GetOwningPlayerState<AGenericGaemPlayerState>();
	const auto Player = GetOwningPlayerPawn<AGenericGaemCharacter>();
	if (!PlayerState || !Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGenericGaemWidget::InitializeWidget: PlayerState is null, going to sleep and restart in a timer"));
		FTimerDelegate InitWidgetDelegate = FTimerDelegate::CreateUObject(this, &UGenericGaemWidget::InitializeWidget, World);
		World->GetTimerManager().SetTimer(_HackyTimerHandle, InitWidgetDelegate, 0.2f, false);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("UGenericGaemWidget::InitializeWidget: PlayerState is not null, yippee"));
	PlayerState->OnMoneyChanged().AddUObject(this, &UGenericGaemWidget::OnMoneyChanged);
	PlayerState->OnRoleChanged().AddUObject(this, &UGenericGaemWidget::OnRoleChanged);
	PlayerState->OnRolePurchased().AddUObject(this, &UGenericGaemWidget::OnRolePurchased);
	PlayerState->OnHealthChanged().AddUObject(this, &UGenericGaemWidget::OnHealthChanged);
	PlayerState->OnSelectedActiveItemChanged().AddUObject(this, &UGenericGaemWidget::OnActiveItemSelectedChanged);
	PlayerState->OnPlayerDeath().AddUObject(this, &UGenericGaemWidget::OnPlayerDeath);
	PlayerState->OnPlayerRevive().AddUObject(this, &UGenericGaemWidget::OnPlayerRevive);
	PlayerState->OnShopDisplayRequested().AddUObject(this, &UGenericGaemWidget::OnShopRequested);
	PlayerState->OnShopDisplayExited().AddUObject(this, &UGenericGaemWidget::OnShopExited);
	PlayerState->OnPlayerShopReload().AddUObject(this, &UGenericGaemWidget::OnShopReload);
	Player->OnEscapeMenu().AddUObject(this, &UGenericGaemWidget::OnEscapeMenu);
}

void UGenericGaemWidget::OnShopReload_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Shop reloaded in GenericGaemWidget"));
}

void UGenericGaemWidget::OnShopRequested_Implementation(AShop* Shop)
{
	UE_LOG(LogTemp, Warning, TEXT("Shop requested in GenericGaemWidget: %s"), *Shop->GetName());
}

void UGenericGaemWidget::OnEscapeMenu_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Escape menu opened in GenericGaemWidget"));
}

void UGenericGaemWidget::OnHealthChanged_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Health changed in GenericGaemWidget"));
}

void UGenericGaemWidget::OnActiveItemSelectedChanged_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Active item selected changed in GenericGaemWidget"));
}

void UGenericGaemWidget::PostLoad()
{
	Super::PostLoad();
}

void UGenericGaemWidget::OnPlayerRevive_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Player revived in GenericGaemWidget"));
}

void UGenericGaemWidget::OnShopExited_Implementation(AShop* Shop)
{
	UE_LOG(LogTemp, Warning, TEXT("Shop exited in GenericGaemWidget: %s"), *Shop->GetName());
}

TArray<FRoleListItem> UGenericGaemWidget::GetRoleListItems() const
{
	TArray<FRoleListItem> RoleListItems{};
	const auto& Roles = FRoleTableRow::GetAllRoles(RoleTable);
	for (const auto& Role : Roles)
	{
		RoleListItems.Push(
			FRoleListItem{
				Role->GetRoleName(),
				Role->GetRolePrice(),
				Role->GetRoleDescription()
			}
		);
	}
	return RoleListItems;
}

void UGenericGaemWidget::OnPlayerDeath_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Player death in GenericGaemWidget"));
}

void UGenericGaemWidget::OnRolePurchased_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Role purchased in GenericGaemWidget"));
}

void UGenericGaemWidget::OnMoneyChanged_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Money changed in GenericGaemWidget"));
}

void UGenericGaemWidget::OnRoleChanged_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Role changed in GenericGaemWidget"));
}