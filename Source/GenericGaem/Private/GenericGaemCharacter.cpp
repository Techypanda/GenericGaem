// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AGenericGaemCharacter::AGenericGaemCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	MaximumZoomValue = 300.0f;
	MinimumZoomValue = 0.0f;
	ZoomMagnitudeValue = 10.0f;
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