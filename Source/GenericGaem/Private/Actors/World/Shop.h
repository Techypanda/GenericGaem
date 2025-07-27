// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shop.generated.h"

UCLASS()
class AShop : public AActor
{
	GENERATED_BODY()
	
public:
	AShop();
	UFUNCTION(BlueprintCallable)
	class AGenericGaemCharacter* GetShopWorker() const;
	UFUNCTION(BlueprintCallable)
	void SetShopWorker(class AGenericGaemCharacter* ShopWorker);
	UFUNCTION(BlueprintCallable)
	void RemoveShopWorker();
	UFUNCTION(BlueprintCallable)
	FString GetShopTitle() const { return _ShopTitle; }
	UFUNCTION(BlueprintCallable)
	TArray<struct FShopListItem> GetItemsForSale() const;
	UFUNCTION(BlueprintCallable)
	FString GetPriceOfItem(FName Row) const;
	UFUNCTION(BlueprintCallable)
	TSubclassOf<ABaseItem> GetItemClass(FName Row) const;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(BlueprintCallable)
	int GetCurrentTier() const { return CurrentTier; }
	UFUNCTION(BlueprintCallable)
	int GetTierCount() const { return CachedTierCount; }
	UFUNCTION(BlueprintCallable)
	void SetCurrentTier(int NewTier);
	UFUNCTION()
	int CalculateStartingTierFromDt() const;
	UFUNCTION()
	int CalculateFinalTierFromDt() const;
	UFUNCTION(BlueprintCallable)
	bool CanUpgradeFurther() const { return bCanUpgradeFurther; }
	void SetCanUpgradeFurther(bool bNewCanUpgrade);
	UFUNCTION(BlueprintCallable)
	FString GetNextUpgradeCost() const { return NextUpgradeCost; }
	void SetNextUpgradeCost(FString NextCost);
protected:
	int CachedTierCount;
	UPROPERTY(Replicated)
	FString NextUpgradeCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	FString _ShopTitle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	UDataTable* Tiers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	UDataTable* ItemsForSale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	class USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	class UBoxComponent* ShopOwnerCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	class UBoxComponent* ShopBuyerCollider;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Shop")
	class UTextRenderComponent* ShopWorkerLabel;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Shop")
	class UTextRenderComponent* TierLabel;
	UPROPERTY(Replicated)
	bool bCanUpgradeFurther;
	UPROPERTY(ReplicatedUsing=OnRep_ShopWorker)
	class AGenericGaemCharacter* _ShopWorker;
	UPROPERTY(ReplicatedUsing=OnRep_CurrentTier)
	int CurrentTier;
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnRep_CurrentTier();
	void OnCurrentTierUpdate();
	UFUNCTION(BlueprintNativeEvent)
	void NativeOnCurrentTierUpdate();
	UFUNCTION()
	void OnRep_ShopWorker();
	UFUNCTION(BlueprintNativeEvent)
	void OnShopWorkerUpdate();
	UFUNCTION()
	void OnShopWorkerColliderEnter(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnShopWorkerColliderExit(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnShopBuyColliderEnter(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnShopBuyColliderExit(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void UpdateTierLabel();
private:
	FString _GetNextUpgradeCost() const;
};
