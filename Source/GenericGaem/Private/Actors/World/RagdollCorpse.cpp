// Fill out your copyright notice in the Description page of Project Settings.

#include "RagdollCorpse.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
ARagdollCorpse::ARagdollCorpse()
{
	UE_LOG(LogTemp, Warning, TEXT("ARagdollCorpse constructor called"));
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	SkeletonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletonMeshComponent"));
	SkeletonMeshComponent->SetupAttachment(SceneComponent);
	SkeletonMeshComponent->SetCollisionProfileName(TEXT("Ragdoll"));
	SkeletonMeshComponent->SetIsReplicated(true);
	SetReplicateMovement(true);
}

void ARagdollCorpse::OnRep_PlayerMesh()
{
	OnPlayerMeshUpdate();
}

void ARagdollCorpse::OnPlayerMeshUpdate()
{
	if (_PlayerMesh)
	{
		SkeletonMeshComponent->SetSkeletalMesh(_PlayerMesh);
		SkeletonMeshComponent->SetSimulatePhysics(true);
	}
}

// Called when the game starts or when spawned
void ARagdollCorpse::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ARagdollCorpse BeginPlay called"));
}

void ARagdollCorpse::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARagdollCorpse, SkeletonMeshComponent);
	DOREPLIFETIME(ARagdollCorpse, _PlayerMesh);
}

// Called every frame
void ARagdollCorpse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARagdollCorpse::CopyPlayer(ACharacter* PlayerCharacter)
{
	_PlayerMesh = PlayerCharacter->GetMesh()->SkeletalMesh;
}
