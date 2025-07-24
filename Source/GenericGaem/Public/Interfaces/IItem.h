// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IItem.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UItem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GENERICGAEM_API IItem
{
	GENERATED_BODY()
public:
	virtual void Use() = 0;
	virtual FString Name() = 0;
};
