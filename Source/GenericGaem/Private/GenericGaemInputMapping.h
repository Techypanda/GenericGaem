// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GenericGaemInputMapping.generated.h"

/**
 * 
 */
UCLASS()
class UGenericGaemInputMapping : public UInputMappingContext
{
	GENERATED_BODY()
public:
	UGenericGaemInputMapping();
	class UInputTriggerReleased* TriggerReleased;
	class UInputTriggerPressed* TriggerPressed;
	class UInputTriggerDown* TriggerDown;
	class UInputModifierNegate* NegateModifier;
	class UInputAction* MoveForwardAction;
	class UInputAction* MoveRightAction;
	class UInputAction* JumpAction;
	class UInputAction* Mouse2DAction;
	class UInputAction* ZoomInOutAction;
	void Initialize();
private:
	void CreateMoveForwardAction();
	void CreateMoveRightAction();
	void CreateJumpAction();
	void CreateMouse2DAction();
	void CreateZoomInOutAction();
};
