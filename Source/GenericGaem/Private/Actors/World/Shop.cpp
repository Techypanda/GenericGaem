// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop.h"
#include "ShopTableRow.h"
#include "ShopTierRow.h"
#include "BaseRole.h"
#include "BaseItem.h"
#include "GenericGaemMode.h"
#include "GenericGaemStructures.h"
#include "GenericGaemCharacter.h"
#include "GenericGaemPlayerState.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerState.h"
#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"

static std::string DefaultShopWorkerText = "No Shop Worker Currently";

AShop::AShop()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	ShopOwnerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ShopOwnerCollisionBox"));
	ShopOwnerCollider->SetupAttachment(SceneComponent);
	ShopOwnerCollider->BodyInstance.SetInstanceNotifyRBCollision(true);
	ShopBuyerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ShopBuyerCollisionBox"));
	ShopBuyerCollider->SetupAttachment(SceneComponent);
	ShopWorkerLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ShopWorkerLabel"));
	ShopWorkerLabel->SetupAttachment(SceneComponent);
	ShopWorkerLabel->SetIsReplicated(true);
	ShopWorkerLabel->SetText(FText::FromString(DefaultShopWorkerText.c_str()));
	TierLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TierLabel"));
	TierLabel->SetupAttachment(SceneComponent);
	TierLabel->SetIsReplicated(true);
	TierLabel->SetText(FText::FromString("Tier ?"));
}

AGenericGaemCharacter* AShop::GetShopWorker() const
{
	return _ShopWorker;
}

void AShop::SetShopWorker(AGenericGaemCharacter* ShopWorker)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetShopWorker can only be called on the server."));
		return;
	}
	if (_ShopWorker)
	{
		UE_LOG(LogTemp, Warning, TEXT("Shop worker already set."));
		return;
	}
	_ShopWorker = ShopWorker;
	OnShopWorkerUpdate();
}

void AShop::OnShopWorkerUpdate_Implementation()
{
	if (GetShopWorker())
	{
		ShopWorkerLabel->SetText(FText::FromString(GetShopWorker()->GetPlayerState()->GetPlayerName()));
	}
	else
	{
		ShopWorkerLabel->SetText(FText::FromString(DefaultShopWorkerText.c_str()));
	}
}

void AShop::RemoveShopWorker()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveShopWorker can only be called on the server."));
		return;
	}
	if (!_ShopWorker)
	{
		UE_LOG(LogTemp, Warning, TEXT("No shop worker to remove."));
		return;
	}
	_ShopWorker = nullptr;
	OnShopWorkerUpdate();
}

TArray<FShopListItem> AShop::GetItemsForSale() const
{
	TArray<FShopListItem> OutArray;
	for (const auto& RowName : ItemsForSale->GetRowNames())
	{
		const auto& Row = ItemsForSale->FindRow<FShopTableRow>(RowName, TEXT("GetItemsForSale"));
		OutArray.Add(FShopListItem{
			.Name = Row->ShopDisplayName,
			.Price = Row->Price,
			.RequiredTier = Row->RequiredTier,
			.RowName = RowName.ToString()
		});
	}
	return OutArray;
}

FString AShop::GetPriceOfItem(FName Row) const
{
	const auto& FoundRow = ItemsForSale->FindRow<FShopTableRow>(Row, TEXT("GetPriceOfItem"));
	if (!FoundRow)
	{
		// Return a huge amount
		return TEXT("999999999");
	}
	return FoundRow->Price;
}

TSubclassOf<ABaseItem> AShop::GetItemClass(FName Row) const
{
	const auto& FoundRow = ItemsForSale->FindRow<FShopTableRow>(Row, TEXT("GetPriceOfItem"));
	return FoundRow->Item;
}

void AShop::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShop, _ShopWorker);
	DOREPLIFETIME(AShop, CurrentTier);
	DOREPLIFETIME(AShop, NextUpgradeCost);
	DOREPLIFETIME(AShop, bCanUpgradeFurther);
	DOREPLIFETIME(AShop, ShopWorkerLabel);
	DOREPLIFETIME(AShop, TierLabel);
}

void AShop::SetCurrentTier(int NewTier)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetCurrentTier can only be called on the server."));
		return;
	}
	CurrentTier = NewTier;
	OnCurrentTierUpdate();
}

int AShop::CalculateStartingTierFromDt() const
{
	TArray<FShopTierRow*> TiersArray;
	Tiers->GetAllRows<FShopTierRow>(TEXT("CalculateStartingTierFromDt"), TiersArray);
	TiersArray.Sort([](const FShopTierRow& A, const FShopTierRow& B)
		{
			return A.TierNumber < B.TierNumber;
		});
	return TiersArray[0]->TierNumber;
}

int AShop::CalculateFinalTierFromDt() const
{
	TArray<FShopTierRow*> TiersArray;
	Tiers->GetAllRows<FShopTierRow>(TEXT("CalculateStartingTierFromDt"), TiersArray);
	TiersArray.Sort([](const FShopTierRow& A, const FShopTierRow& B)
		{
			return A.TierNumber < B.TierNumber;
		});
	return TiersArray[TiersArray.Num() - 1]->TierNumber;
}

void AShop::SetCanUpgradeFurther(bool bNewCanUpgrade)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetCanUpgradeFurther can only be called on the server."));
		return;
	}
	bCanUpgradeFurther = bNewCanUpgrade;
}

void AShop::SetNextUpgradeCost(FString NextCost)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetNextUpgradeCost can only be called on the server."));
		return;
	}
	NextUpgradeCost = NextCost;
}

void AShop::BeginPlay()
{
	Super::BeginPlay();
	CachedTierCount = CalculateFinalTierFromDt();
	if (GetLocalRole() == ROLE_Authority)
	{
		SetCurrentTier(CalculateStartingTierFromDt());
		SetCanUpgradeFurther(CurrentTier < CachedTierCount);
		SetNextUpgradeCost(_GetNextUpgradeCost());
	}
	ShopOwnerCollider->OnComponentBeginOverlap.Clear();
	ShopOwnerCollider->OnComponentBeginOverlap.AddDynamic(this, &AShop::OnShopWorkerColliderEnter);
	ShopOwnerCollider->OnComponentEndOverlap.Clear();
	ShopOwnerCollider->OnComponentEndOverlap.AddDynamic(this, &AShop::OnShopWorkerColliderExit);
	ShopBuyerCollider->OnComponentBeginOverlap.Clear();
	ShopBuyerCollider->OnComponentBeginOverlap.AddDynamic(this, &AShop::OnShopBuyColliderEnter);
	ShopBuyerCollider->OnComponentEndOverlap.Clear();
	ShopBuyerCollider->OnComponentEndOverlap.AddDynamic(this, &AShop::OnShopBuyColliderExit);
	UpdateTierLabel();
}

void AShop::OnRep_CurrentTier()
{
	OnCurrentTierUpdate();
}

void AShop::OnCurrentTierUpdate()
{
	UE_LOG(LogTemp, Warning, TEXT("AAAA: %hs"), CurrentTier < CachedTierCount ? "true" : "false");
	SetCanUpgradeFurther(CurrentTier < CachedTierCount);
	SetNextUpgradeCost(_GetNextUpgradeCost());
	UpdateTierLabel();
	NativeOnCurrentTierUpdate();
	UE_LOG(LogTemp, Warning, TEXT("Shop current tier updated to: %d"), CurrentTier);
}

void AShop::NativeOnCurrentTierUpdate_Implementation()
{
}

void AShop::OnRep_ShopWorker()
{
	OnShopWorkerUpdate();
}

void AShop::OnShopWorkerColliderEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnShopWorkerColliderEnter: %s"), *OtherActor->GetName());
	const auto& NewShopWorker = Cast<AGenericGaemCharacter>(OtherActor);
	if (NewShopWorker)
	{
		SetShopWorker(NewShopWorker);
	}
}

void AShop::OnShopWorkerColliderExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const auto& ExitingShopWorker = Cast<AGenericGaemCharacter>(OtherActor);
	if (!ExitingShopWorker)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Exiting shop worker: %s"), *ExitingShopWorker->GetName());
	if (_ShopWorker && ExitingShopWorker && _ShopWorker->GetPlayerState()->GetPlayerId() == ExitingShopWorker->GetPlayerState()->GetPlayerId()) // _ShopWorker->GetPlayerState()->GetPlayerId() == ExitingShopWorker->GetPlayerState()->GetPlayerId()
	{
		UE_LOG(LogTemp, Warning, TEXT("Exiting shop worker 2: %s"), *_ShopWorker->GetName());
		RemoveShopWorker();
	}
}

void AShop::OnShopBuyColliderEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto& Buyer = Cast<AGenericGaemCharacter>(OtherActor);
	if (Buyer)
	{
		Buyer->GetPlayerState<AGenericGaemPlayerState>()->OnShopDisplayRequested().Broadcast(this);
	}
}

void AShop::OnShopBuyColliderExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const auto& Buyer = Cast<AGenericGaemCharacter>(OtherActor);
	if (Buyer)
	{
		Buyer->GetPlayerState<AGenericGaemPlayerState>()->OnShopDisplayExited().Broadcast(this);
	}
}

void AShop::UpdateTierLabel()
{
	const auto& TierLabelText = FString::Printf(TEXT("Tier: %d/%d"), GetCurrentTier(), GetTierCount());
	TierLabel->SetText(FText::FromString(TierLabelText));
}

FString AShop::_GetNextUpgradeCost() const
{
	TArray<FShopTierRow*> TiersArray;
	Tiers->GetAllRows<FShopTierRow>(TEXT("CalculateStartingTierFromDt"), TiersArray);
	TiersArray.Sort([](const FShopTierRow& A, const FShopTierRow& B)
		{
			return A.TierNumber < B.TierNumber;
		});
	for (const auto& Tier : TiersArray)
	{
		if (Tier->TierNumber > CurrentTier)
		{
			return Tier->Price;
		}
	}
	return "9999999999";
}