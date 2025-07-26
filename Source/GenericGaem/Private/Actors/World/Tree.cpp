// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

static constexpr float DefaultTreeHealth = 100.0f;
static constexpr float FruitTreeHealth = 50.0f;
static constexpr float MoneyTreeHealth = 300.0f;
static constexpr float DefaultTreeMoney = 10.0f;
static constexpr float FruitTreeMoney = 5.0f;
static constexpr float MoneyTreeMoney = 150.0f;
static constexpr float DefaultTreeRespawnTime = 30.0f; // 30 seconds

// Sets default values
ATree::ATree() : _Health{DefaultTreeHealth}
{
	bReplicates = true;
	bAlwaysRelevant = true;
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
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
	CollisionBox->SetIsReplicated(true);
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

void ATree::SetHealth(float NewHealth)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	const float MaxHealth = bIsMoneyTree ? MoneyTreeHealth : bIsFruitTree ? FruitTreeHealth : DefaultTreeHealth;
	_Health = std::clamp(NewHealth, 0.0f, MaxHealth);
}

float ATree::GetHealth() const
{
	return _Health;
}

float ATree::BreakTree()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return 0.0f;
	}
	bTreeDead = true;
	OnTreeDead();
	GetWorld()->GetTimerManager().SetTimer(_RespawnTreeTimer, this, &ATree::RespawnTree, DefaultTreeRespawnTime, false);
	return bIsMoneyTree ? MoneyTreeMoney : bIsFruitTree ? FruitTreeMoney : DefaultTreeMoney;
}

void ATree::RespawnTree()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Respawning tree %s"), *GetName());
	InitialTreeLogic();
	bTreeDead = false;
	OnTreeDead();
}

void ATree::InitialTreeLogic()
{
	// First Roll For Money Tree (note: strange bug going on here where clients dont get true from Set function)
	const auto& MoneyTreeRoll = UKismetMathLibrary::RandomIntegerInRange(0, 100) <= static_cast<int>(MoneyTreeChance * 100);
	if (SetIsMoneyTree(MoneyTreeRoll) || MoneyTreeRoll)
	{
		SetHealth(MoneyTreeHealth);
		return;
	}
	// Then Roll Fruit Tree
	const auto& FruitTreeRoll = UKismetMathLibrary::RandomIntegerInRange(0, 100) <= static_cast<int>(FruitTreeChance * 100);
	if (SetIsFruitTree(FruitTreeRoll) || FruitTreeRoll)
	{
		SetHealth(FruitTreeHealth);
		return;
	}
	SetHealth(DefaultTreeHealth);
	// Fallback on tree mesh
	RandomizeTreeMesh();
}

void ATree::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATree, bIsMoneyTree);
	DOREPLIFETIME(ATree, bIsFruitTree);
	DOREPLIFETIME(ATree, _Health);
	DOREPLIFETIME(ATree, bTreeDead);
}

// Called when the game starts or when spawned
void ATree::BeginPlay()
{
	Super::BeginPlay();
	checkf(MoneyTreeChance <= 1.0f && MoneyTreeChance >= 0.0f, TEXT("MoneyTreeChance must be between 0 and 1"));
	checkf(FruitTreeChance <= 1.0f && FruitTreeChance >= 0.0f, TEXT("FruitTreeChance must be between 0 and 1"));
	InitialTreeLogic();
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

void ATree::OnRep_Health()
{
	OnHealthUpdate();
}

void ATree::OnHealthUpdate()
{
	if (GetLocalRole() == ROLE_Authority)
	{
	}
}

void ATree::OnRep_TreeDead()
{
	OnTreeDead();
}

void ATree::OnTreeDead()
{
	if (bTreeDead)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionBox->SetCollisionProfileName(TEXT("NoCollision"));
		SelectedTreeMesh->SetVisibility(false, true);
	}
	else
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CollisionBox->SetCollisionProfileName(TEXT("BlockAllDynamic"));
		SelectedTreeMesh->SetVisibility(true, true);
	}
}

void ATree::RandomizeTreeMesh()
{
	checkf(TreeMeshes.Num() > 0, TEXT("TreeMeshes need to be allocated"));
	const auto& _RandomTreeMesh = TreeMeshes[UKismetMathLibrary::RandomIntegerInRange(0, TreeMeshes.Num() - 1)];
	SelectedTreeMesh->SetStaticMesh(_RandomTreeMesh.LoadSynchronous());
}