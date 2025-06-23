#include "GenericGaemLighting.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

const uint16 AGenericGaemLighting::StartLightPitch = 0;
const uint16 AGenericGaemLighting::EndLightPitch = 181;
const uint8 AGenericGaemLighting::StartingHour = 1; // Start at 1 AM

AGenericGaemLighting::AGenericGaemLighting()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
	DayDirectionalLight = NewDirectionalLight(TEXT("DayDirectionalLight"), EDayNightLightIndex::Day);
	NightDirectionalLight = NewDirectionalLight(TEXT("NightDirectionalLight"), EDayNightLightIndex::Night);
	ExponentialHeightFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("ExponentialHeightFog"));
	ExponentialHeightFog->SetupAttachment(RootComponent);
	ExponentialHeightFog->SetFogInscatteringColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
	ExponentialHeightFog->SetDirectionalInscatteringColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
	HourOfDay = StartingHour;
	SetupDay();
}

UDirectionalLightComponent* AGenericGaemLighting::NewDirectionalLight(FName Name, EDayNightLightIndex Index)
{
	const auto DirectionalLight = CreateDefaultSubobject<UDirectionalLightComponent>(Name);
	DirectionalLight->SetupAttachment(RootComponent);
	DirectionalLight->bAtmosphereSunLight = true;
	DirectionalLight->AtmosphereSunLightIndex = static_cast<int32>(Index);
	return DirectionalLight;
}

void AGenericGaemLighting::SwapShadingPriority(class UDirectionalLightComponent* Priority, class UDirectionalLightComponent* LowPriority)
{
	Priority->ForwardShadingPriority = 1.0f;
	LowPriority->ForwardShadingPriority = 0.0f;
}

void AGenericGaemLighting::SetupNight()
{
	SwapShadingPriority(NightDirectionalLight, DayDirectionalLight);
	DayDirectionalLight->SetRelativeRotation(FRotator(0, 0, 0));
	NightDirectionalLight->SetRelativeRotation(FRotator(-181.0, 0, 0));
}

void AGenericGaemLighting::SetupDay()
{
	SwapShadingPriority(DayDirectionalLight, NightDirectionalLight);
	DayDirectionalLight->SetRelativeRotation(FRotator(-181.0, 0, 0));
	NightDirectionalLight->SetRelativeRotation(FRotator(0, 0, 0));
}

// Called when the game starts or when spawned
void AGenericGaemLighting::BeginPlay()
{
	Super::BeginPlay();
	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Display, TEXT("Server Start (BeginPlay), Now Initializing Time Of Day To: %d"), StartingHour);
		HourOfDay = StartingHour;
		OnHourOfDayUpdate();
	}
}

void AGenericGaemLighting::OnHourOfDayUpdate()
{
	if (GetLocalRole() == ROLE_Authority)
	{
	}
	if (HourOfDay > 12) {
		UE_LOG(LogTemp, Warning, TEXT("Its day now: %d"), HourOfDay);
		SetupDay();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Its night now: %d"), HourOfDay);
		SetupNight();
	}
}

void AGenericGaemLighting::OnRep_HourOfDayUpdate()
{
	OnHourOfDayUpdate();
}

void AGenericGaemLighting::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGenericGaemLighting, HourOfDay);
}