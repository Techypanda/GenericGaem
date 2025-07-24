// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GenericGaemState.generated.h"

/**
 * 
 */
UCLASS()
class GENERICGAEM_API AGenericGaemState : public AGameStateBase
{
	GENERATED_BODY()
public:
	void SetLeader(int32 InLeaderId);

protected:
	UPROPERTY(ReplicatedUsing = OnRep_Leader)
	int32 LeaderId;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void PreInitializeComponents() override;

	UFUNCTION()
	void OnRep_Leader();
	void OnLeaderUpdate();
};
