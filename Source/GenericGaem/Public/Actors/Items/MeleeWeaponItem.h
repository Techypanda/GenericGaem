// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MeleeWeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class GENERICGAEM_API AMeleeWeaponItem : public ABaseItem
{
	GENERATED_BODY()
public:
	virtual void Use() override;
	virtual void Attack();
	UFUNCTION(Server, Unreliable)
	virtual void ServerAttack(FVector ActorForwardVector);
	// Maybe in future we will need something like this
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GenericGaem/Weapon", meta=(DisplayName = "OnHit"))
	//virtual void ReceiveOnHit()
protected:
	UFUNCTION(BlueprintCallable)
	AActor* RaytraceForHit(class AGenericGaemCharacter* _Character, FVector ActorForwardVector, bool drawDebugLine = false) const;
	UFUNCTION(BlueprintCallable)
	class AGenericGaemCharacter* GetCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HitDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HealthDamage;
};
