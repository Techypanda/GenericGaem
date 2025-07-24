// Fill out your copyright notice in the Description page of Project Settings.
#include "GenericGaemMainMenuWidget.h"
#include <Kismet/GameplayStatics.h>
#include "Engine/Level.h"
#include "GenericGaemMainMenuPlayerControl.h"

const FName UGenericGaemMainMenuWidget::MainLevelName = TEXT("/Game/Levels/L_PersistentLevel");

void UGenericGaemMainMenuWidget::OnQuitButtonClicked()
{
	const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->ConsoleCommand("quit", true);
}

void UGenericGaemMainMenuWidget::OnHostButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), UGenericGaemMainMenuWidget::MainLevelName, true, "listen");
}

void UGenericGaemMainMenuWidget::OnJoinButtonClicked()
{
}

void UGenericGaemMainMenuWidget::OnConnectButtonClicked(FString IpAddress)
{
	auto IpAddressWithPort = FString::Printf(TEXT("%s:7777%s"), *IpAddress, *MainLevelName.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Connecting to IP: %s"), *IpAddressWithPort);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->ClientTravel(IpAddressWithPort, ETravelType::TRAVEL_Absolute);
}
