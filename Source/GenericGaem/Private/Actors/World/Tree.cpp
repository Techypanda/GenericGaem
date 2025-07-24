// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATree::ATree()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(RootComponent);
	SelectedTreeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TreeMesh"));
	SelectedTreeMesh->SetupAttachment(SceneComponent);
	SelectedTreeMesh->SetIsReplicated(true);
	if (TreeMeshes.Num() > 0) {
		SelectedTreeMesh->SetStaticMesh(TreeMeshes[0].LoadSynchronous());
	}
	SelectedTreeMesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f)); // Default scale
	SelectedTreeMesh->SetCollisionProfileName(TEXT("NoCollision"));
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SimpleCollisionBox"));
	CollisionBox->SetupAttachment(SelectedTreeMesh);
	CollisionBox->SetWorldScale3D(FVector(3.5f, 3.5f, 12.5f));
	CollisionBox->SetWorldLocation(FVector(0.0f, 0.0f, 400.0f));
	CollisionBox->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

bool ATree::SetIsMoneyTree(bool bInMoneyTree)
{
	checkf(FruitTreeMesh.IsNull() == false, TEXT("Money Tree Mesh Not Specified"))
	if (GetLocalRole() == ROLE_Authority)
	{
		bIsMoneyTree = bInMoneyTree;
		if (bIsMoneyTree) {
			SelectedTreeMesh->SetStaticMesh(MoneyTreeMesh.LoadSynchronous());
		}
		OnMoneyTreeUpdate();
	}
	return bInMoneyTree;
}

bool ATree::GetIsMoneyTree() const
{
	return bIsMoneyTree;
}

bool ATree::SetIsFruitTree(bool bInFruitTree)
{
	checkf(FruitTreeMesh.IsNull() == false, TEXT("Fruit Tree Mesh Not Specified"))
	if (GetLocalRole() == ROLE_Authority)
	{
		bIsFruitTree = bInFruitTree;
		if (bIsFruitTree) {
			SelectedTreeMesh->SetStaticMesh(FruitTreeMesh.LoadSynchronous());
		}
		OnFruitTreeUpdate();
	}
	return bInFruitTree;
}

bool ATree::GetIsFruitTree() const
{
	return bIsFruitTree;
}

void ATree::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATree, bIsMoneyTree);
	DOREPLIFETIME(ATree, bIsFruitTree);
}

// Called when the game starts or when spawned
void ATree::BeginPlay()
{
	Super::BeginPlay();
	checkf(MoneyTreeChance <= 1.0f && MoneyTreeChance >= 0.0f, TEXT("MoneyTreeChance must be between 0 and 1"));
	checkf(FruitTreeChance <= 1.0f && FruitTreeChance >= 0.0f, TEXT("FruitTreeChance must be between 0 and 1"));
	// First Roll For Money Tree (note: strange bug going on here where clients dont get true from Set function)
	const auto& MoneyTreeRoll = UKismetMathLibrary::RandomIntegerInRange(0, 100) <= static_cast<int>(MoneyTreeChance * 100);
	if (SetIsMoneyTree(MoneyTreeRoll) || MoneyTreeRoll)
	{
		return;
	}
	// Then Roll Fruit Tree
	const auto& FruitTreeRoll = UKismetMathLibrary::RandomIntegerInRange(0, 100) <= static_cast<int>(FruitTreeChance * 100);
	if (SetIsFruitTree(FruitTreeRoll) || FruitTreeRoll)
	{
		return;
	}
	// Fallback on tree mesh
	RandomizeTreeMesh();
}

void ATree::OnRep_IsMoneyTree()
{
	OnMoneyTreeUpdate();
}

void ATree::OnMoneyTreeUpdate()
{
	if (GetLocalRole() == ROLE_Authority)
	{
	}
}

void ATree::OnRep_IsFruitTree()
{
	OnFruitTreeUpdate();
}

void ATree::OnFruitTreeUpdate()
{
	if (GetLocalRole() == ROLE_Authority)
	{
	}
}

void ATree::RandomizeTreeMesh()
{
	checkf(TreeMeshes.Num() > 0, TEXT("TreeMeshes need to be allocated"));
	const auto& _RandomTreeMesh = TreeMeshes[UKismetMathLibrary::RandomIntegerInRange(0, TreeMeshes.Num() - 1)];
	SelectedTreeMesh->SetStaticMesh(_RandomTreeMesh.LoadSynchronous());
}