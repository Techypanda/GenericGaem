// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericGaemState.h"

void AGenericGaemState::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	// Custom initialization logic can be added here
	UE_LOG(LogTemp, Warning, TEXT("Server Travel URL: %s"), *GetWorld()->URL.ToString());
}