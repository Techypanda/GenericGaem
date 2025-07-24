// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericGaemLighting.generated.h"

UCLASS()
class AGenericGaemLighting : public AActor
{
	GENERATED_BODY()
	
public:
	const static uint16 StartLightPitch;
	// Must Be Negated (* -1)
	const static uint16 EndLightPitch;
	const static uint8 StartingHour;
	const static uint8 EndHourOfNight = 24;
	const static uint8 EndHourOfDay = 12;

	// Sets default values for this actor's properties
	AGenericGaemLighting();

	UPROPERTY(EditAnywhere, Category = "Lighting")
	class UDirectionalLightComponent* DayDirectionalLight;

	UPROPERTY(EditAnywhere, Category = "Lighting")
	class UDirectionalLightComponent* NightDirectionalLight;

	UPROPERTY(EditAnywhere, Category = "Lighting")
	class UExponentialHeightFogComponent* ExponentialHeightFog;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing=OnRep_HourOfDayUpdate)
	uint8 HourOfDay;

	UFUNCTION()
	void OnRep_HourOfDayUpdate();

private:
	enum class EDayNightLightIndex
	{
		Day = 0,
		Night = 1
	};
	class UDirectionalLightComponent* NewDirectionalLight(FName Name, EDayNightLightIndex Index);
	void SetupNight();
	void SetupDay();
	void SwapShadingPriority(class UDirectionalLightComponent* Priority, class UDirectionalLightComponent* LowPriority);
	void OnHourOfDayUpdate();
};
