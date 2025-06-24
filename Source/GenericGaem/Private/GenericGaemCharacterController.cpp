#include "GenericGaemCharacterController.h"

AGenericGaemCharacterController::AGenericGaemCharacterController()
{
}

void AGenericGaemCharacterController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);
}

void AGenericGaemCharacterController::BeginPlay()
{
	Super::BeginPlay();
}

void AGenericGaemCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
}