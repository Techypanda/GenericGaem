// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingMeshTest.generated.h"

UCLASS()
class ARotatingMeshTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatingMeshTest();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
	class UStaticMeshComponent* ImageMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Image")
	class USceneComponent* SceneComponent;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
