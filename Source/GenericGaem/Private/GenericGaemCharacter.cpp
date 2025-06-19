// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AGenericGaemCharacter::AGenericGaemCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	CameraSpringArmComponent->TargetArmLength = 0.0f; // Default to FPS view
	GetMesh()->bOwnerNoSee = true; // Hide mesh in fps
	GetMesh()->MarkRenderStateDirty();
	bIsFirstPerson = true;
}

void AGenericGaemCharacter::SetThirdPerson()
{
	CameraSpringArmComponent->bUsePawnControlRotation = false; // In third person, player should use right click to turn camera
	GetMesh()->bOwnerNoSee = false; // Show mesh in third person
	GetMesh()->MarkRenderStateDirty();
	bIsFirstPerson = false;
}

// Called when the game starts or when spawned
void AGenericGaemCharacter::BeginPlay()
{
	Super::BeginPlay();
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