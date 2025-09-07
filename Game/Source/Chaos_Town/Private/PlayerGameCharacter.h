// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCharacter.h"
#include "PlayerGameCharacter.generated.h"

/**
 * 
 */
UCLASS()
class APlayerGameCharacter : public AGameCharacter
{
	GENERATED_BODY()
public:
	/// <inheritdoc />
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/// <inheritdoc />
	virtual void BeginPlay() override;
protected:
	/// <summary>
	/// Input Mapping Context for Ground Locomotion
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<class UInputMappingContext> _GroundLocomotionInputMapping;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<class UInputAction> _MoveXAxisAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<class UInputAction> _MoveYAxisAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<class UInputAction> _JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<class UInputAction> _MouseXYAction;
private:
	/// <summary>
	/// Loads/Unloads the Input Mapping Context for Ground Locomotion
	/// </summary>
	/// <param name="bLoad">True to load, false to unload</param>
	void _LoadGroundLocomotionMapping(bool bLoad);
	/// <summary>
	/// Binds the input actions to relevant functions for player
	/// </summary>
	void _BindInputs(class UEnhancedInputComponent& InputComponent);
	/// <summary>
	/// Binding function for moving on the X axis
	/// </summary>
	void _MoveXAxis(const struct FInputActionInstance& Instance);
	/// <summary>
	/// Binding function for moving on the Y axis
	/// </summary>
	void _MoveYAxis(const struct FInputActionInstance& Instance);
	/// <summary>
	/// Calls AddMovementInput with the axis input and value
	/// </summary>
	void _MovementXY(const float& Value, const EAxis::Type& Axis);
	/// <summary>
	/// Binding function for moving on the XY axis for mouse input
	/// </summary>
	void _MouseXY(const struct FInputActionInstance& Instance);
};
