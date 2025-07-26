// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TimerHandle.h"
#include "Tree.generated.h"

UCLASS()
class ATree : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ATree();
	UFUNCTION(BlueprintCallable, Category = "Tree")
	bool SetIsMoneyTree(bool bInMoneyTree);
	UFUNCTION(BlueprintCallable, Category = "Tree")
	bool GetIsMoneyTree() const;
	UFUNCTION(BlueprintCallable, Category = "Tree")
	bool SetIsFruitTree(bool bInFruitTree);
	UFUNCTION(BlueprintCallable, Category = "Tree")
	bool GetIsFruitTree() const;
	UFUNCTION(BlueprintCallable, Category = "Tree")
	void SetHealth(float NewHealth);
	UFUNCTION(BlueprintCallable, Category = "Tree")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Tree")
	float BreakTree();
	void RespawnTree();
	void InitialTreeLogic();
protected:
	FTimerHandle _RespawnTreeTimer;
	UPROPERTY(ReplicatedUsing = OnRep_TreeDead)
	bool bTreeDead;
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	float _Health;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(ReplicatedUsing = OnRep_IsMoneyTree)
	bool bIsMoneyTree;
	UPROPERTY(ReplicatedUsing = OnRep_IsFruitTree)
	bool bIsFruitTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	float MoneyTreeChance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	float FruitTreeChance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	TArray<TSoftObjectPtr<class UStaticMesh>> TreeMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	TSoftObjectPtr<class UStaticMesh> MoneyTreeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	TSoftObjectPtr<class UStaticMesh> FruitTreeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	class UStaticMeshComponent* SelectedTreeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tree")
	class USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	class UBoxComponent* CollisionBox;
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnRep_IsMoneyTree();
	void OnMoneyTreeUpdate();
	UFUNCTION()
	void OnRep_IsFruitTree();
	void OnFruitTreeUpdate();
	UFUNCTION()
	void OnRep_Health();
	void OnHealthUpdate();
	UFUNCTION()
	void OnRep_TreeDead();
	void OnTreeDead();
private:
	void RandomizeTreeMesh();
};
