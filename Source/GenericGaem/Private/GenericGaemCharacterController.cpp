#include "GenericGaemCharacterController.h"
#include "GenericGaemCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GenericGaemInputMapping.h"
#define GUARD_PLAYER_NULL if (!Player) return;

void AGenericGaemCharacterController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);
	Player = Cast<AGenericGaemCharacter>(PossessedPawn);
}

void AGenericGaemCharacterController::BeginPlay()
{
	Super::BeginPlay();
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
	}
	else if (Player->bIsFirstPerson && SpringArm->TargetArmLength > 0)
	{
		Player->SetThirdPerson();
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

void AGenericGaemCharacterController::Look2D(const FInputActionInstance& Instance)
{
	GUARD_PLAYER_NULL
	if (!Player->bIsFirstPerson)
	{
		return;
	}
	const FVector2D Value = Instance.GetValue().Get<FVector2D>();
	Player->AddControllerYawInput(Value.X);
	Player->AddControllerPitchInput(Value.Y * -1);
}