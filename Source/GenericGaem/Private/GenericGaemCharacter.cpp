// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemCharacter.h"
#include "GenericGaemPlayerState.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GenericGaemInputMapping.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/TextRenderComponent.h"
#include <string>

static constexpr float RoleDisplayZOffset = 100.0f;
static constexpr float HealthDisplayZOffset = 115.0f;
static constexpr float RoleDisplayFontsize = 16.0f;
static constexpr float HealthDisplayFontsize = 16.0f;

// Sets default values
AGenericGaemCharacter::AGenericGaemCharacter() : RcMouseX(0), RcMouseY(0), bIsHoldingRightClickInThirdPerson(false), MaximumZoomValue(300.0f), MinimumZoomValue(0.0f), ZoomMagnitudeValue(10.0f), bAllowZoom(true), bDisableMovement(false), bDisableLook(false), bBindedTextRender{false}
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	GetCharacterMovement()->SetIsReplicated(true);
	GetMesh()->SetIsReplicated(true);
	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraSpringArmComponent);
	RoleDisplayComponent = CreateTextRenderComponent(TEXT("RoleDisplay"), RoleDisplayZOffset, RoleDisplayFontsize);
	HealthDisplayComponent = CreateTextRenderComponent(TEXT("HealthDisplay"), HealthDisplayZOffset, HealthDisplayFontsize);
	SetFirstPerson();
}

UTextRenderComponent* AGenericGaemCharacter::CreateTextRenderComponent(const wchar_t* const Name, const float& ZOffset, const float& FontSize)
{
	const auto& NewTextDisplay = CreateDefaultSubobject<UTextRenderComponent>(Name);
	NewTextDisplay->SetupAttachment(RootComponent);
	NewTextDisplay->SetRelativeLocation(FVector(0.0f, 0.0f, ZOffset));
	NewTextDisplay->SetHorizontalAlignment(EHTA_Center);
	NewTextDisplay->SetVerticalAlignment(EVRTA_TextCenter);
	NewTextDisplay->SetText(FText::FromString(""));
	NewTextDisplay->SetIsReplicated(true);
	NewTextDisplay->SetWorldSize(FontSize);
	return NewTextDisplay;
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
	Input->BindAction(LoadedMapping->EscapeMenuAction, ETriggerEvent::Triggered, this, &AGenericGaemCharacter::EscapeMenu);
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

void AGenericGaemCharacter::EnableMovement(bool bEnabled)
{
	bDisableLook = !bEnabled;
	bDisableMovement = !bEnabled;
	bAllowZoom = bEnabled;
}

void AGenericGaemCharacter::ShowCursor(bool bShowCursor)
{
	Cast<APlayerController>(GetController())->bShowMouseCursor = bShowCursor;
}

void AGenericGaemCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	BindTextRenders();
}

void AGenericGaemCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	BindTextRenders();
}

void AGenericGaemCharacter::BindTextRenders()
{
	const auto _PlayerState = GetPlayerState<AGenericGaemPlayerState>();
	if (_PlayerState && !bBindedTextRender)
	{
		bBindedTextRender = true; // I think I think a mutex here as i'm not sure if PossessedBy and OnRep_PlayerState run on different threads, based off docs they dont so don't need
		UE_LOG(LogTemp, Warning, TEXT("Player State Possessed: %s"), *_PlayerState->GetPlayerName());
		_PlayerState->OnRoleChanged().AddUObject(this, &AGenericGaemCharacter::OnRoleChange);
		_PlayerState->OnHealthChanged().AddUObject(this, &AGenericGaemCharacter::OnHealthChange);
		OnRoleChange(); // Get Initial Role
		OnHealthChange(); // Get Initial health
	}
}

void AGenericGaemCharacter::Zoom(const FInputActionInstance& Instance)
{
	if (!bAllowZoom)
	{
		return;
	}
	const float Value = Instance.GetValue().Get<float>();
	USpringArmComponent* SpringArm = GetCameraSpringArmComponent();
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength - Value * ZoomMagnitude(), MinimumZoom(), MaximumZoom());
	if (!bIsFirstPerson && SpringArm->TargetArmLength <= 0)
	{
		SetFirstPerson();
		ShowCursor(false);
	}
	else if (bIsFirstPerson && SpringArm->TargetArmLength > 0)
	{
		SetThirdPerson();
		ShowCursor(true);
	}
}

void AGenericGaemCharacter::EscapeMenu(const FInputActionInstance& Instance)
{
	_EscapeMenuEvent.Broadcast();
}

void AGenericGaemCharacter::MoveForward(const FInputActionInstance& Instance)
{
	if (bDisableMovement)
	{
		return;
	}
	const float Value = Instance.GetValue().Get<float>();
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	// Zero out Z and normalize
	Direction.Z = 0.0f;
	Direction = Direction.GetSafeNormal();
	AddMovementInput(Direction, Value);
}

void AGenericGaemCharacter::MoveRight(const FInputActionInstance& Instance)
{
	if (bDisableMovement)
	{
		return;
	}
	const float Value = Instance.GetValue().Get<float>();
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	// Zero out Z and normalize
	Direction.Z = 0.0f;
	Direction = Direction.GetSafeNormal();
	AddMovementInput(Direction, Value);
}

void AGenericGaemCharacter::Jump(const FInputActionInstance& Instance)
{
	if (bDisableMovement)
	{
		return;
	}
	const bool Value = Instance.GetValue().Get<bool>();
	bPressedJump = Value;
}

void AGenericGaemCharacter::OnRoleChange()
{
	const auto& _PlayerState = GetPlayerState<AGenericGaemPlayerState>();
	const auto& _Role = ERoleHelper::ERoleToRole(_PlayerState->GetGameRole());
	RoleDisplayComponent->SetText(FText::FromString(_Role->GetRoleName().data()));
	RoleDisplayComponent->SetTextRenderColor(_Role->GetRoleColor());
}

void AGenericGaemCharacter::OnHealthChange()
{
	const auto& _PlayerState = GetPlayerState<AGenericGaemPlayerState>();
	const auto& _Health = static_cast<int>(std::ceil(_PlayerState->GetHealth()));
	const auto& _FormattedHealth = std::format("HP: {}", _Health);
	HealthDisplayComponent->SetText(FText::FromString(_FormattedHealth.c_str()));
	if (_Health >= 75)
	{
		HealthDisplayComponent->SetTextRenderColor(FColor::Green);
	}
	else if (_Health >= 35)
	{
		HealthDisplayComponent->SetTextRenderColor(FColor::Orange);
	}
	else
	{
		HealthDisplayComponent->SetTextRenderColor(FColor::Red);
	}
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
	if ((!bIsFirstPerson && !bIsHoldingRightClickInThirdPerson) || bDisableLook)
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