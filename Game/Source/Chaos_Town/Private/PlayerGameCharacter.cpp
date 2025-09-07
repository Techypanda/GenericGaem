#include "PlayerGameCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Math/Axis.h"
#include "GameFramework/PlayerController.h"

void APlayerGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		_BindInputs(*Input);
	}
}

void APlayerGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	_LoadGroundLocomotionMapping(true);
}

void APlayerGameCharacter::_LoadGroundLocomotionMapping(bool bLoad)
{
	const auto& _PlayerController = Cast<APlayerController>(GetController());
	if (!_PlayerController) return;
	if (const auto& _LocalPlayer = _PlayerController->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = _LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!_GroundLocomotionInputMapping.IsNull())
			{
				InputSystem->AddMappingContext(_GroundLocomotionInputMapping.LoadSynchronous(), 0);
			}
		}
	}
}

void APlayerGameCharacter::_BindInputs(UEnhancedInputComponent& _InputComponent)
{
	if (!_MoveXAxisAction.IsNull())
	{
		_InputComponent.BindAction(_MoveXAxisAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayerGameCharacter::_MoveXAxis);
	}
	if (!_MoveYAxisAction.IsNull())
	{
		_InputComponent.BindAction(_MoveYAxisAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayerGameCharacter::_MoveYAxis);
	}
	if (!_JumpAction.IsNull())
	{
		_InputComponent.BindAction(_JumpAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ACharacter::Jump);
	}
	if (!_MouseXYAction.IsNull())
	{
		_InputComponent.BindAction(_MouseXYAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayerGameCharacter::_MouseXY);
	}
}

void APlayerGameCharacter::_MoveXAxis(const FInputActionInstance& Instance)
{
	_MovementXY(Instance.GetValue().Get<float>(), EAxis::X);
}

void APlayerGameCharacter::_MoveYAxis(const FInputActionInstance& Instance)
{
	_MovementXY(Instance.GetValue().Get<float>(), EAxis::Y);
}

void APlayerGameCharacter::_MovementXY(const float& Value, const EAxis::Type& Axis)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(Axis);
	// Zero out Z and normalize
	Direction.Z = 0.0f;
	Direction = Direction.GetSafeNormal();
	AddMovementInput(Direction, Value);
}

void APlayerGameCharacter::_MouseXY(const FInputActionInstance& Instance)
{
	const FVector2D& MouseData = Instance.GetValue().Get<FVector2D>();
	AddControllerPitchInput(MouseData.Y * -1);
	AddControllerYawInput(MouseData.X);
}
