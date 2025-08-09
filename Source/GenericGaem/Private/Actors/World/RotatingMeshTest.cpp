// Fill out your copyright notice in the Description page of Project Settings.
#include "RotatingMeshTest.h"
#include "Kismet/KismetMathLibrary.h"
#include "GenericGaemCharacter.h"

// Sets default values
ARotatingMeshTest::ARotatingMeshTest()
{
	PrimaryActorTick.bCanEverTick = true;
	// note: doesn't need to tick on server.
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	ImageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ImageMesh"));
	ImageMesh->SetupAttachment(SceneComponent);
	ImageMesh->SetWorldLocation(FVector{0.0, 0.0, 50.0});
	ImageMesh->SetWorldRotation(FRotator(0.0f, 0.0f, 90.0f));
}

// Called when the game starts or when spawned
void ARotatingMeshTest::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARotatingMeshTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!GetWorld()->GetFirstPlayerController())
	{
		// Still Rendering
		return;
	}
	const auto& PlayerPawn = Cast<AGenericGaemCharacter>(GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld())->GetPawn());
	if (PlayerPawn)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Player Pawn: %s"), *PlayerPawn->GetName());
		FVector Object1Location = GetActorLocation();
		FVector Object2Location = PlayerPawn->GetCameraLocation();
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Object1Location, Object2Location);
		TargetRotation.Yaw -= 90.0f;
		TargetRotation.Pitch = 0.0f;
		SetActorRotation(TargetRotation);
	}
}

