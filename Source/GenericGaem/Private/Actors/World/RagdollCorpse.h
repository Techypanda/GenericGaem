// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RagdollCorpse.generated.h"

UCLASS()
class ARagdollCorpse : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARagdollCorpse();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void CopyPlayer(class ACharacter* PlayerCharacter);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
	class USceneComponent* SceneComponent;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player")
	class USkeletalMeshComponent* SkeletonMeshComponent;
	UPROPERTY(ReplicatedUsing=OnRep_PlayerMesh)
	class USkeletalMesh* _PlayerMesh;
	UFUNCTION()
	void OnRep_PlayerMesh();
	void OnPlayerMeshUpdate();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
