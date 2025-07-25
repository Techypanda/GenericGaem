// Fill out your copyright notice in the Description page of Project Settings.
#include "GenericGaemState.h"
#include <string>
#include "DeathObject.h"
#include "StarterItemsTableRow.h"
#include "DataRole.h"
#include "RoleTableRow.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

void AGenericGaemState::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	// Custom initialization logic can be added here
	UE_LOG(LogTemp, Warning, TEXT("Server Travel URL: %s"), *GetWorld()->URL.ToString());
}

void AGenericGaemState::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AGenericGaemState, LeaderId);
}

void AGenericGaemState::OnLeaderUpdate()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("Leader updated to: %d"), LeaderId);
	}
}

void AGenericGaemState::OnRep_Leader()
{
	OnLeaderUpdate();
}

void AGenericGaemState::BeginPlay()
{
	Super::BeginPlay();
}

void AGenericGaemState::SetLeader(int32 InLeaderId)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		LeaderId = InLeaderId;
		OnLeaderUpdate();
	}
}

const TSubclassOf<ADeathObject> AGenericGaemState::GetDeathObject() const
{
	return DeathObject;
}