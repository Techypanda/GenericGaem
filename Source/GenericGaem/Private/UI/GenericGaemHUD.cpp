// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemHUD.h"
#include "GenericGaemWidget.h"
#include "GenericGaemPlayerState.h"
#include "Blueprint/UserWidget.h"

AGenericGaemHUD::AGenericGaemHUD() : _GenericGaemWidgetInstance{nullptr}
{ }

void AGenericGaemHUD::BeginPlay()
{
	Super::BeginPlay();
	const auto LoadedGenericGaemWidget = _GenericGaemWidgetClass.LoadSynchronous();
	_GenericGaemWidgetInstance = CreateWidget<UGenericGaemWidget>(GetWorld(), LoadedGenericGaemWidget);
	if (!_GenericGaemWidgetInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("GenericGaemWidget instance didn't create successfully."));
		return;
	}
	_GenericGaemWidgetInstance->InitializeWidget(GetWorld());
	_GenericGaemWidgetInstance->AddToViewport();
}

const UGenericGaemWidget* AGenericGaemHUD::GetGenericGaemWidget() const
{
	return _GenericGaemWidgetInstance;
}