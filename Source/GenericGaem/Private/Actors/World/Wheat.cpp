// Fill out your copyright notice in the Description page of Project Settings.

#include "Wheat.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

static constexpr int64 WheatStageDuration = 20; // 2 Minutes For Each Stage 

// Sets default values
AWheat::AWheat()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	WheatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheatMesh"));
	WheatMesh->SetupAttachment(SceneComponent);
	WheatMesh->SetIsReplicated(true);
	WheatMesh->SetStaticMesh(Stage_01_Mesh.Get());
	WheatMesh->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f)); // Default scale
	WheatMesh->SetCollisionProfileName(TEXT("NoCollision"));
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SimpleCollisionBox"));
	CollisionBox->SetupAttachment(WheatMesh);
	CollisionBox->SetWorldScale3D(FVector(12.0f, 18.25f, 20.5f));
	CollisionBox->SetWorldLocation(FVector(-21.0f, 0.0f, 620.0f));
	CollisionBox->SetIsReplicated(true);
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AWheat::SetNextStageEpoch(int64 NewEpoch)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return; // Only the server can set the next stage epoch
	}
	// GuaranteedNextStageEpoch = NewEpoch;
}

void AWheat::SetStage(EWheatStage NewStage)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return; // Only the server can set the stage
	}
	// _Stage = NewStage;
	OnStageUpdate();
}

void AWheat::BeginPlay()
{
	Super::BeginPlay();
	SetStage(EWheatStage::StageOne); // Initialize to Stage One
}

void AWheat::OnRep_NextStage()
{
	OnStageUpdate();
}

void AWheat::OnStageUpdate()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		SetNextStageEpoch(ComputeNextStageEpoch());
	}
	// _SetMeshToStage(_Stage);
}

void AWheat::_SetMeshToStage(EWheatStage _InStage)
{
	// could use a map, this is just a simple switch for now
	switch (_InStage)
	{
	case EWheatStage::StageOne:
		WheatMesh->SetStaticMesh(Stage_01_Mesh.Get());
		break;
	case EWheatStage::StageTwo:
		WheatMesh->SetStaticMesh(Stage_02_Mesh.Get());
		break;
	default:
		WheatMesh->SetStaticMesh(Stage_03_Mesh.Get());
		break;
	}
}

const int64 AWheat::ComputeNextStageEpoch() const
{
	return FDateTime::Now().ToUnixTimestamp() + WheatStageDuration;
}

//void AWheat::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//	UE_LOG(LogTemp, Warning, TEXT("Wheat Stage: %d"), static_cast<int32>(_Stage));
//	if (GetLocalRole() == ROLE_Authority)
//	{
//		if (_Stage != EWheatStage::StageThree && FDateTime::Now().ToUnixTimestamp() >= GuaranteedNextStageEpoch)
//		{
//			EWheatStage NextStage = static_cast<EWheatStage>((static_cast<uint8>(_Stage) + 1));
//			SetStage(NextStage);
//		}
//	}
//}

//void AWheat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	// DOREPLIFETIME(AWheat, GuaranteedNextStageEpoch);
//	// DOREPLIFETIME(AWheat, _Stage);
//}

