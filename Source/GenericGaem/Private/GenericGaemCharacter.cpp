// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GenericGaemInputMapping.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "IRole.h"
#include "BaseRole.h"

// Sets default values
AGenericGaemCharacter::AGenericGaemCharacter() : RcMouseX(0), RcMouseY(0), bIsHoldingRightClickInThirdPerson(false), MaximumZoomValue(300.0f), MinimumZoomValue(0.0f), ZoomMagnitudeValue(10.0f), AssignedRole(std::make_unique<BaseRole>())
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	GetCharacterMovement()->SetIsReplicated(true);
	GetMesh()->SetIsReplicated(true);
	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraSpringArmComponent);
	SetFirstPerson();
}

void AGenericGaemCharacter::SetFirstPerson()
{
	CameraSpringArmComponent->bUsePawnControlRotation = true; // In FPS view the camera rotates with the character
	bUseControllerRotationYaw = true;
	if (UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent()))
	{
		MovementComponent->bOrientRotationToMovement = false;
	}
	CameraSpringArmComponent->TargetArmLength = 0.0f; // Default to FPS view
	GetMesh()->bOwnerNoSee = true; // Hide mesh in fps
	GetMesh()->MarkRenderStateDirty();
	bIsFirstPerson = true;
}

void AGenericGaemCharacter::SetThirdPerson()
{
	CameraSpringArmComponent->bUsePawnControlRotation = true; // In third person, player should use right click to turn camera
	bUseControllerRotationYaw = false;
	if (UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent()))
	{
		MovementComponent->bOrientRotationToMovement = true;
	}
	GetMesh()->bOwnerNoSee = false; // Show mesh in third person
	GetMesh()->MarkRenderStateDirty();
	bIsFirstPerson = false;
}

// Called when the game starts or when spawned
void AGenericGaemCharacter::BeginPlay()
{
	Super::BeginPlay();
	bIsHoldingRightClickInThirdPerson = false;
	SetFirstPerson();
}

// Called every frame
void AGenericGaemCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGenericGaemCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UGenericGaemInputMapping* LoadedMapping = nullptr;
	const auto Player = GetWorld()->GetFirstLocalPlayerFromController();
	UE_LOG(LogTemp, Warning, TEXT("Local Player: %s"), *Player->GetName());
	if (UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		if (!InputMapping.IsNull())
		{
			LoadedMapping = InputMapping.LoadSynchronous();
			LoadedMapping->Initialize();
			InputSystem->AddMappingContext(LoadedMapping, 1);
			UE_LOG(LogTemp, Warning, TEXT("Input Mapping Context Loaded"));
		}
	}
	if (!LoadedMapping)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load Input Mapping Context"));
		return;
	}
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
	Input->BindAction(LoadedMapping->ZoomInOutAction, ETriggerEvent::Triggered, this, &AGenericGaemCharacter::Zoom);
	Input->BindAction(LoadedMapping->Mouse2DAction, ETriggerEvent::Triggered, this, &AGenericGaemCharacter::Look2D);
	Input->BindAction(LoadedMapping->MoveForwardAction, ETriggerEvent::Triggered, this, &AGenericGaemCharacter::MoveForward);
	Input->BindAction(LoadedMapping->MoveRightAction, ETriggerEvent::Triggered, this, &AGenericGaemCharacter::MoveRight);
	Input->BindAction(LoadedMapping->JumpAction, ETriggerEvent::Triggered, this, &AGenericGaemCharacter::Jump);
	Input->BindAction(LoadedMapping->ThirdPersonMouse2D, ETriggerEvent::Triggered, this, &AGenericGaemCharacter::ThirdPersonRightClick);
}

USpringArmComponent* AGenericGaemCharacter::GetCameraSpringArmComponent()
{
	return CameraSpringArmComponent;
}

const float AGenericGaemCharacter::MinimumZoom() const
{
	return MinimumZoomValue;
}

const float AGenericGaemCharacter::MaximumZoom() const
{
	return MaximumZoomValue;
}

const float AGenericGaemCharacter::ZoomMagnitude() const
{
	return ZoomMagnitudeValue;
}

const IRole& AGenericGaemCharacter::GetRole() const
{
	return *AssignedRole.get();
}

void AGenericGaemCharacter::Zoom(const FInputActionInstance& Instance)
{
	const float Value = Instance.GetValue().Get<float>();
	USpringArmComponent* SpringArm = GetCameraSpringArmComponent();
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength - Value * ZoomMagnitude(), MinimumZoom(), MaximumZoom());
	if (!bIsFirstPerson && SpringArm->TargetArmLength <= 0)
	{
		SetFirstPerson();
		Cast<APlayerController>(GetController())->bShowMouseCursor = false;
	}
	else if (bIsFirstPerson && SpringArm->TargetArmLength > 0)
	{
		SetThirdPerson();
		Cast<APlayerController>(GetController())->bShowMouseCursor = true;
	}
}

void AGenericGaemCharacter::MoveForward(const FInputActionInstance& Instance)
{
	const float Value = Instance.GetValue().Get<float>();
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	// Zero out Z and normalize
	Direction.Z = 0.0f;
	Direction = Direction.GetSafeNormal();
	AddMovementInput(Direction, Value);
}

void AGenericGaemCharacter::MoveRight(const FInputActionInstance& Instance)
{
		const float Value = Instance.GetValue().Get<float>();
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	// Zero out Z and normalize
	Direction.Z = 0.0f;
	Direction = Direction.GetSafeNormal();
	AddMovementInput(Direction, Value);
}

void AGenericGaemCharacter::Jump(const FInputActionInstance& Instance)
{
	const bool Value = Instance.GetValue().Get<bool>();
	bPressedJump = Value;
}

void AGenericGaemCharacter::ThirdPersonRightClick(const FInputActionInstance& Instance)
{
	const bool Value = Instance.GetValue().Get<bool>();
	bIsHoldingRightClickInThirdPerson = !bIsFirstPerson && Value; // TODO: optimize this, its only called twice but its still a inefficient call pre sure, if theres a way to change mappings for first person/third person we can avoid a whole two calls.
	const auto PlayerController = Cast<APlayerController>(GetController());
	PlayerController->GetMousePosition(RcMouseX, RcMouseY);
}

void AGenericGaemCharacter::Look2D(const FInputActionInstance& Instance)
{
	// Move camera with right click in third person
	if (!bIsFirstPerson && !bIsHoldingRightClickInThirdPerson)
	{
		return;
	}
	const FVector2D Value = Instance.GetValue().Get<FVector2D>();
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y * -1);
	if (!bIsFirstPerson && bIsHoldingRightClickInThirdPerson)
	{
		// need to instead do delta things...
		// the mouse does not move when rotating
		// SetMouseLocation(static_cast<int32>(RcMouseX), static_cast<int32>(RcMouseY));
	}
}