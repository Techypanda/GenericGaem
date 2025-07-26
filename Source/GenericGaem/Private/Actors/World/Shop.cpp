// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop.h"
#include "ShopTableRow.h"
#include "GenericGaemStructures.h"
#include "GenericGaemCharacter.h"
#include "GenericGaemPlayerState.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

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
	ShopOwnerCollider->OnComponentBeginOverlap.AddDynamic(this, &AShop::OnShopWorkerColliderEnter);
	ShopOwnerCollider->OnComponentEndOverlap.AddDynamic(this, &AShop::OnShopWorkerColliderExit);
	ShopBuyerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ShopBuyerCollisionBox"));
	ShopBuyerCollider->SetupAttachment(SceneComponent);
	ShopBuyerCollider->OnComponentBeginOverlap.AddDynamic(this, &AShop::OnShopBuyColliderEnter);
	ShopBuyerCollider->OnComponentEndOverlap.AddDynamic(this, &AShop::OnShopBuyColliderExit);
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

void AShop::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShop, _ShopWorker);
	DOREPLIFETIME(AShop, CurrentTier);
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

void AShop::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Shop BeginPlay: %d"), StartingTier);
	SetCurrentTier(StartingTier);
	ShopOwnerCollider->OnComponentBeginOverlap.Clear();
	ShopOwnerCollider->OnComponentBeginOverlap.AddDynamic(this, &AShop::OnShopWorkerColliderEnter);
	ShopOwnerCollider->OnComponentEndOverlap.Clear();
	ShopOwnerCollider->OnComponentEndOverlap.AddDynamic(this, &AShop::OnShopWorkerColliderExit);
	ShopBuyerCollider->OnComponentBeginOverlap.Clear();
	ShopBuyerCollider->OnComponentBeginOverlap.AddDynamic(this, &AShop::OnShopBuyColliderEnter);
	ShopBuyerCollider->OnComponentEndOverlap.Clear();
	ShopBuyerCollider->OnComponentEndOverlap.AddDynamic(this, &AShop::OnShopBuyColliderExit);
}

void AShop::OnRep_CurrentTier()
{
	OnCurrentTierUpdate();
}

void AShop::OnCurrentTierUpdate_Implementation()
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

void AShop::OnShopWorkerUpdate_Implementation()
{
}
