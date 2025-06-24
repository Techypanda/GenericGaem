// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemMainMenuGameMode.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"

void AGenericGaemMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("GenericGaemMainMenuGameMode::BeginPlay called"));
	if (!MainMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuWidget is not set in GenericGaemMainMenuGameMode"));
		return;
	}
	auto InstancedMainMenuWidget = CreateWidget(GetWorld(), MainMenuWidget);
	if (!InstancedMainMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create MainMenuWidget instance"));
		return;
	}
	 InstancedMainMenuWidget->AddToViewport();
}