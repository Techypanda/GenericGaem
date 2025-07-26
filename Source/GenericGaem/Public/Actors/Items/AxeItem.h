// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeWeaponItem.h"
#include "AxeItem.generated.h"

/**
 * 
 */
UCLASS()
class GENERICGAEM_API AAxeItem : public AMeleeWeaponItem
{
	GENERATED_BODY()
public:
	virtual void ServerAttack_Implementation(FVector ActorForwardVector) override;
};
