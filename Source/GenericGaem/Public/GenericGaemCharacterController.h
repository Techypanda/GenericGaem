// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericGaemCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class GENERICGAEM_API AGenericGaemCharacterController : public APlayerController
{
	GENERATED_BODY()
public:
	void OnPossess(APawn* aPawn) override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TSoftObjectPtr<class UGenericGaemInputMapping> InputMapping;
	void BeginPlay() override;
	void SetupInputComponent() override;
private:
	class AGenericGaemCharacter* Player;
	void MoveForward(const struct FInputActionInstance& Instance);
	void MoveRight(const struct FInputActionInstance& Instance);
	void Look2D(const struct FInputActionInstance& Instance);
	void Jump(const struct FInputActionInstance& Instance);
	void Zoom(const struct FInputActionInstance& Instance);
};
