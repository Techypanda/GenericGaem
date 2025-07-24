// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <memory>
#include "DeathObject.generated.h"

UCLASS()
class ADeathObject : public AActor
{
	GENERATED_BODY()
public:	
	ADeathObject();
	void Tick(float DeltaSeconds) override;
	void SetDeath(const float& RespawnTime, const FString PlayerName);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetDeath(const float& RespawnTime, const FString& PlayerName);
protected:
	// Currently a skull
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* SkullTopMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* SkullBottomMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	class UTextRenderComponent* DeathPlayerLabel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	class UTextRenderComponent* DeathRespawnLabel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
	class USceneComponent* SceneComponent;
	class UTextRenderComponent* CreateTextRenderComponent(const wchar_t* const Name, const float& ZOffset, const FColor& Color, const float& FontSize);
private:
	std::unique_ptr<float> _RespawnTime;
};
