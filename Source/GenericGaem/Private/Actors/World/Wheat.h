// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wheat.generated.h"

UENUM(BlueprintType)
enum EWheatStage : uint8
{
	StageOne = 0,
	StageTwo = 1,
	StageThree = 2
};

UCLASS()
class AWheat : public AActor
{
	GENERATED_BODY()
	
public:
	AWheat();

	// void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// UFUNCTION(BlueprintCallable)
	// const int64 GetNextStageEpoch() const { return GuaranteedNextStageEpoch; }
	void SetNextStageEpoch(int64 NewEpoch);
	void SetStage(EWheatStage NewStage);
	// UFUNCTION(BlueprintCallable)
	// const EWheatStage GetStage() const { return _Stage; }
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheat")
	class UStaticMeshComponent* WheatMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheat")
	class UBoxComponent* CollisionBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wheat")
	class USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheat/Stages")
	TWeakObjectPtr<class UStaticMesh> Stage_01_Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheat/Stages")
	TWeakObjectPtr<class UStaticMesh> Stage_02_Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheat/Stages")
	TWeakObjectPtr<class UStaticMesh> Stage_03_Mesh;
	// UPROPERTY(Replicated)
	// int64 GuaranteedNextStageEpoch;
	// UPROPERTY(ReplicatedUsing=OnRep_NextStage)
	// TEnumAsByte<EWheatStage> _Stage;
	UFUNCTION()
	void OnRep_NextStage();
	UFUNCTION()
	void OnStageUpdate();
private:
	void _SetMeshToStage(EWheatStage _InStage);
	const int64 ComputeNextStageEpoch() const;
};
