// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IItem.h"
#include "BaseItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GENERICGAEM_API ABaseItem : public AActor, public IItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName;
	ABaseItem();
	virtual FString Name() override;
	virtual void Use() override;
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
