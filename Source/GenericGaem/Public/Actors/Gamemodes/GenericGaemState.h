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
	void BeginPlay() override;
	void SetLeader(int32 InLeaderId);
	const TSubclassOf<class ADeathObject> GetDeathObject() const;
	// const TArray<TSubclassOf<class ABaseItem>> GetStarterItemsForRole(ERole Role) const;
protected:
	UPROPERTY(ReplicatedUsing = OnRep_Leader)
	int32 LeaderId;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void PreInitializeComponents() override;

	UFUNCTION()
	void OnRep_Leader();
	void OnLeaderUpdate();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Death);
	TSubclassOf<class ADeathObject> DeathObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GenericGaemMode")
	UDataTable* RoleStarterItems;
};
