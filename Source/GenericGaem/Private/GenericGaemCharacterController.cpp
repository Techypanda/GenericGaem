#include "GenericGaemCharacterController.h"
#include "GenericGaemCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GenericGaemInputMapping.h"
#include <string_view>
#define GUARD_PLAYER_NULL if (!Player) return;

AGenericGaemCharacterController::AGenericGaemCharacterController()
{
	bIsHoldingRightClickInThirdPerson = false;
	RcMouseX = 0;
	RcMouseY = 0;
}

void AGenericGaemCharacterController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);
	Player = Cast<AGenericGaemCharacter>(PossessedPawn);
}

void AGenericGaemCharacterController::BeginPlay()
{
	Super::BeginPlay();
	bIsHoldingRightClickInThirdPerson = false;
}

void AGenericGaemCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UGenericGaemInputMapping* LoadedMapping = nullptr;
	// no great docs around this yet, im sure you can do this onpossess instead somehow...
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetLocalPlayer()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMapping.IsNull())
			{
				LoadedMapping = InputMapping.LoadSynchronous();
				LoadedMapping->Initialize();
				InputSystem->AddMappingContext(LoadedMapping, 1);
				UE_LOG(LogTemp, Warning, TEXT("Input Mapping Context Loaded"));
			}
		}
	}
	if (!LoadedMapping)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load Input Mapping Context"));
		return;
	}
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
	Input->BindAction(LoadedMapping->MoveForwardAction, ETriggerEvent::Triggered, this, &AGenericGaemCharacterController::MoveForward);
	Input->BindAction(LoadedMapping->MoveRightAction, ETriggerEvent::Triggered, this, &AGenericGaemCharacterController::MoveRight);
	Input->BindAction(LoadedMapping->JumpAction, ETriggerEvent::Triggered, this, &AGenericGaemCharacterController::Jump);
	Input->BindAction(LoadedMapping->Mouse2DAction, ETriggerEvent::Triggered, this, &AGenericGaemCharacterController::Look2D);
	Input->BindAction(LoadedMapping->ZoomInOutAction, ETriggerEvent::Triggered, this, &AGenericGaemCharacterController::Zoom);
	Input->BindAction(LoadedMapping->ThirdPersonMouse2D, ETriggerEvent::Triggered, this, &AGenericGaemCharacterController::ThirdPersonRightClick);
}

void AGenericGaemCharacterController::Zoom(const FInputActionInstance& Instance)
{
	GUARD_PLAYER_NULL
	const float Value = Instance.GetValue().Get<float>();
	USpringArmComponent* SpringArm = Player->GetCameraSpringArmComponent();
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength - Value * Player->ZoomMagnitude(), Player->MinimumZoom(), Player->MaximumZoom());
	UE_LOG(LogTemp, Warning, TEXT("Zooming: %hs, Length: %f"), Player->bIsFirstPerson ? "true" : "false", SpringArm->TargetArmLength);
	if (!Player->bIsFirstPerson && SpringArm->TargetArmLength <= 0)
	{
		Player->SetFirstPerson();
		bShowMouseCursor = false;
	}
	else if (Player->bIsFirstPerson && SpringArm->TargetArmLength > 0)
	{
		Player->SetThirdPerson();
		bShowMouseCursor = true;
	}
}

void AGenericGaemCharacterController::MoveForward(const FInputActionInstance& Instance)
{
	GUARD_PLAYER_NULL
	const float Value = Instance.GetValue().Get<float>();
	FVector Direction = FRotationMatrix(Player->Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	// Zero out Z and normalize
	Direction.Z = 0.0f;
	Direction = Direction.GetSafeNormal();
	Player->AddMovementInput(Direction, Value);
}

void AGenericGaemCharacterController::MoveRight(const FInputActionInstance& Instance)
{
	GUARD_PLAYER_NULL
	const float Value = Instance.GetValue().Get<float>();
	FVector Direction = FRotationMatrix(Player->Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	// Zero out Z and normalize
	Direction.Z = 0.0f;
	Direction = Direction.GetSafeNormal();
	Player->AddMovementInput(Direction, Value);
}

void AGenericGaemCharacterController::Jump(const FInputActionInstance& Instance)
{
	GUARD_PLAYER_NULL
	const bool Value = Instance.GetValue().Get<bool>();
	Player->bPressedJump = Value;
}

void AGenericGaemCharacterController::ThirdPersonRightClick(const FInputActionInstance& Instance)
{
	GUARD_PLAYER_NULL
	const bool Value = Instance.GetValue().Get<bool>();
	bIsHoldingRightClickInThirdPerson = !Player->bIsFirstPerson && Value; // TODO: optimize this, its only called twice but its still a inefficient call pre sure, if theres a way to change mappings for first person/third person we can avoid a whole two calls.
	GetMousePosition(RcMouseX, RcMouseY);
}

void AGenericGaemCharacterController::Look2D(const FInputActionInstance& Instance)
{
	GUARD_PLAYER_NULL
	// Move camera with right click in third person
	if (!Player->bIsFirstPerson && !bIsHoldingRightClickInThirdPerson)
	{
		return;
	}
	const FVector2D Value = Instance.GetValue().Get<FVector2D>();
	Player->AddControllerYawInput(Value.X);
	Player->AddControllerPitchInput(Value.Y * -1);
	if (!Player->bIsFirstPerson && bIsHoldingRightClickInThirdPerson)
	{
		// need to instead do delta things...w
		// the mouse does not move when rotating
		// SetMouseLocation(static_cast<int32>(RcMouseX), static_cast<int32>(RcMouseY));
	}
}