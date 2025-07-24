// Fill out your copyright notice in the Description page of Project Settings.

#include "DeathObject.h"
#include "Components/TextRenderComponent.h"
#include <cmath>

static constexpr float SkullZOffset = 5.0f;
static constexpr float PlayerLabelZOffset = 40.0f;
static constexpr float RespawnLabelZOffset = 25.0f;
static constexpr float PlayerLabelFontSize = 24.0f;
static constexpr float RespawnLabelFontSize = 16.0f;
static constexpr float ExtraLifespan = 3.0f;

ADeathObject::ADeathObject()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.3f;
	bReplicates = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SkullTopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkullTopMesh"));
	SkullTopMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Assets/Models/human_skull/StaticMeshes/Object_3")));
	SkullBottomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkullBottomMesh"));
	SkullBottomMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Assets/Models/human_skull/StaticMeshes/Object_2")));
	DeathPlayerLabel = CreateTextRenderComponent(TEXT("DeathPlayerLabel"), PlayerLabelZOffset, FColor::Red, PlayerLabelFontSize);
	DeathRespawnLabel = CreateTextRenderComponent(TEXT("DeathRespawnLabel"), RespawnLabelZOffset, FColor::Blue, RespawnLabelFontSize);
	SceneComponent->SetupAttachment(RootComponent);
	SkullTopMesh->SetupAttachment(SceneComponent);
	SkullBottomMesh->SetupAttachment(SkullTopMesh);
	DeathPlayerLabel->SetupAttachment(SkullTopMesh);
	DeathRespawnLabel->SetupAttachment(SkullTopMesh);
	SkullTopMesh->SetRelativeLocation(FVector(0.0f, 0.0f, SkullZOffset));
	
}

void ADeathObject::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (_RespawnTime) {
		*_RespawnTime.get() = std::fmax(*_RespawnTime.get() - DeltaSeconds, 0);
		const auto _RoundedSeconds = std::ceil(*_RespawnTime.get());
		DeathRespawnLabel->SetText(FText::Format(FText::FromString("Respawns in {0} seconds"), FText::AsNumber(_RoundedSeconds)));
	}
}

void ADeathObject::SetDeath(const float& RespawnTime, const FString PlayerName)
{
	_RespawnTime = std::make_unique<float>(RespawnTime);
	// Give a bit of extra time
	UE_LOG(LogTemp, Warning, TEXT("Setting death for player %s with respawn time %.2f seconds"), *PlayerName, RespawnTime);
	SetLifeSpan(RespawnTime + ExtraLifespan);
	DeathPlayerLabel->SetText(FText::FromString(PlayerName));
	const auto _RoundedSeconds = std::roundf(RespawnTime * 100) / 100;
	DeathRespawnLabel->SetText(FText::Format(FText::FromString("Respawns in {0} seconds"), FText::AsNumber(_RoundedSeconds)));
}

void ADeathObject::MulticastSetDeath_Implementation(const float& RespawnTime, const FString& PlayerName)
{
	SetDeath(RespawnTime, PlayerName);
}

//void ADeathObject::BeginPlay()
//{
//	Super::BeginPlay();
//	UE_LOG(LogTemp, Warning, TEXT("Death Object BeginPlay called!"));
//	SetDeath(10.0f, TEXT("JONATHAN"));
//}

UTextRenderComponent* ADeathObject::CreateTextRenderComponent(const wchar_t* const Name, const float& ZOffset, const FColor& Color, const float& FontSize)
{
	const auto& NewTextDisplay = CreateDefaultSubobject<UTextRenderComponent>(Name);
	NewTextDisplay->SetHorizontalAlignment(EHTA_Center);
	NewTextDisplay->SetVerticalAlignment(EVRTA_TextCenter);
	NewTextDisplay->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	NewTextDisplay->SetIsReplicated(true);
	NewTextDisplay->SetRelativeLocation(FVector(0.0f, 0.0f, ZOffset));
	NewTextDisplay->SetTextRenderColor(Color);
	NewTextDisplay->SetWorldSize(FontSize);
	return NewTextDisplay;
}