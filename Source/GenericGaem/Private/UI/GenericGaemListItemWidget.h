// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GenericGaemListItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class UGenericGaemListItemWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintCallable)
	bool SelectRole(int32 ERoleVal);
};
