#include "GenericGaemInputMapping.h"
#include "InputAction.h"

UGenericGaemInputMapping::UGenericGaemInputMapping()
{
	UE_LOG(LogTemp, Warning, TEXT("GenericGaemInputMapping Constructor Called"));
}

void UGenericGaemInputMapping::Initialize()
{
	TriggerPressed = NewObject<UInputTriggerPressed>(this, TEXT("TriggerPressed"));
	TriggerReleased = NewObject<UInputTriggerReleased>(this, TEXT("TriggerReleased"));
	TriggerDown = NewObject<UInputTriggerDown>(this, TEXT("TriggerDown"));
	NegateModifier = NewObject<UInputModifierNegate>(this, TEXT("NegationModifier"));
	CreateMoveForwardAction();
	CreateMoveRightAction();
	CreateJumpAction();
	CreateMouse2DAction();
	CreateZoomInOutAction();
}

void UGenericGaemInputMapping::CreateMoveForwardAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Creating MoveForwardAction in GenericGaemInputMapping"));
	MoveForwardAction = NewObject<UInputAction>(this, TEXT("A_MoveForward"));
	MoveForwardAction->ValueType = EInputActionValueType::Axis1D;
	MoveForwardAction->ActionDescription = FText::FromString("Handle Moving Forward (And Backward)");
	auto FwMapping = FEnhancedActionKeyMapping(MoveForwardAction, EKeys::W);
	auto BwMapping = FEnhancedActionKeyMapping(MoveForwardAction, EKeys::S);
	FwMapping.Triggers.Add(TriggerDown);
	BwMapping.Modifiers.Add(NegateModifier);
	BwMapping.Triggers.Add(TriggerDown);
	Mappings.Add(FwMapping);
	Mappings.Add(BwMapping);
}

void UGenericGaemInputMapping::CreateMoveRightAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Creating CreateMoveRightAction in GenericGaemInputMapping"));
	MoveRightAction = NewObject<UInputAction>(this, TEXT("A_MoveRight"));
	MoveRightAction->ValueType = EInputActionValueType::Axis1D;
	MoveRightAction->ActionDescription = FText::FromString("Handle Moving Right (And Left)");
	auto FwMapping = FEnhancedActionKeyMapping(MoveRightAction, EKeys::D);
	auto BwMapping = FEnhancedActionKeyMapping(MoveRightAction, EKeys::A);
	FwMapping.Triggers.Add(TriggerDown);
	BwMapping.Modifiers.Add(NegateModifier);
	BwMapping.Triggers.Add(TriggerDown);
	Mappings.Add(FwMapping);
	Mappings.Add(BwMapping);
}

void UGenericGaemInputMapping::CreateJumpAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Creating CreateJumpAction in GenericGaemInputMapping"));
	JumpAction = NewObject<UInputAction>(this, TEXT("A_Jump"));
	JumpAction->ValueType = EInputActionValueType::Boolean;
	JumpAction->ActionDescription = FText::FromString("Handle Jumping");
	auto StartMapping = FEnhancedActionKeyMapping(JumpAction, EKeys::SpaceBar);
	auto EndMapping = FEnhancedActionKeyMapping(JumpAction, EKeys::SpaceBar);
	EndMapping.Modifiers.Add(NegateModifier);
	StartMapping.Triggers.Add(TriggerPressed);
	EndMapping.Triggers.Add(TriggerReleased);
	Mappings.Add(StartMapping);
	Mappings.Add(EndMapping);
}

void UGenericGaemInputMapping::CreateMouse2DAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Creating CreateMouseXAction in GenericGaemInputMapping"));
	Mouse2DAction = NewObject<UInputAction>(this, TEXT("A_Mouse2D"));
	Mouse2DAction->ValueType = EInputActionValueType::Axis2D;
	Mouse2DAction->ActionDescription = FText::FromString("Handle Mouse XY Movement");
	auto Mouse2DMapping = FEnhancedActionKeyMapping(Mouse2DAction, EKeys::Mouse2D);
	Mappings.Add(Mouse2DMapping);
}

void UGenericGaemInputMapping::CreateZoomInOutAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Creating CreateZoomInOutAction in GenericGaemInputMapping"));
	ZoomInOutAction = NewObject<UInputAction>(this, TEXT("A_ZoomInOut"));
	ZoomInOutAction->ValueType = EInputActionValueType::Axis1D;
	ZoomInOutAction->ActionDescription = FText::FromString("Handle Zooming In and Out");
	auto ZoomInMapping = FEnhancedActionKeyMapping(ZoomInOutAction, EKeys::MouseWheelAxis);
	Mappings.Add(ZoomInMapping);
}