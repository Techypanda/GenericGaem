// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GenericGaemHUD.generated.h"

/**
 * 
 */
UCLASS()
class AGenericGaemHUD : public AHUD
{
	GENERATED_BODY()
public:
	AGenericGaemHUD();
	void BeginPlay() override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSoftClassPtr<class UGenericGaemWidget> _GenericGaemWidgetClass;
	UFUNCTION()
	const UGenericGaemWidget* GetGenericGaemWidget() const;
private:
	UGenericGaemWidget* _GenericGaemWidgetInstance;
};
