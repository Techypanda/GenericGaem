// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericGaemCharacter.generated.h"

UCLASS()
class GENERICGAEM_API AGenericGaemCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera);
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera);
	class USpringArmComponent* CameraSpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera);
	float MinimumZoomValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera);
	float MaximumZoomValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera);
	float ZoomMagnitudeValue;
	virtual void BeginPlay() override;

public:
	bool bIsFirstPerson;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	AGenericGaemCharacter();

	void SetFirstPerson();

	void SetThirdPerson();

	USpringArmComponent* GetCameraSpringArmComponent();

	const float MinimumZoom() const;

	const float MaximumZoom() const;

	const float ZoomMagnitude() const;
};
