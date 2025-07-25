// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum ERoleName : uint8
{
	None = 0 UMETA(DisplayName="None"),
	Peasant UMETA(DisplayName = "Peasant"),
	Citizen UMETA(DisplayName = "Citizen"),
	Monkey UMETA(DisplayName = "Monkey"),
	Leader UMETA(DisplayName = "Leader")
};

class ERoleHelper
{
public:
	static TMap<ERoleName, FString> ERoleNameToString;
	static TMap<FString, ERoleName> StringToERoleName;
};