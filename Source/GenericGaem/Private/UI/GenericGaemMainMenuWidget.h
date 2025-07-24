// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GenericGaemMainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class UGenericGaemMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	const static FName MainLevelName;
	UFUNCTION(BlueprintCallable, Category = "GenericGaem|MainMenu")
	void OnQuitButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "GenericGaem|MainMenu")
	void OnHostButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "GenericGaem|MainMenu")
	void OnJoinButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "GenericGaem|MainMenu")
	void OnConnectButtonClicked(FString IpAddress);
};
