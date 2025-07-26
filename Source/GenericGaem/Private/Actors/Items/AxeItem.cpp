// Fill out your copyright notice in the Description page of Project Settings.


#include "AxeItem.h"
#include "Tree.h"
#include "GenericGaemPlayerState.h"
#include "GenericGaemCharacterController.h"
#include "GenericGaemCharacter.h"
#include "GenericGaemMode.h"

void AAxeItem::ServerAttack_Implementation(FVector ActorForwardVector)
{
	UE_LOG(LogTemp, Warning, TEXT("AAxeItem::ServerAttack_Implementation called"));
	const auto& _Character = GetCharacter();
	const auto _World = _Character->GetWorld();
	const auto& HitActor = RaytraceForHit(_Character, ActorForwardVector, true);
	if (!HitActor)
	{
		return;
	}
	const auto _Tree = Cast<ATree>(HitActor);
	if (!_Tree)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit actor is not a Tree: %s"), *HitActor->GetName());
		return;
	}
	_Tree->SetHealth(_Tree->GetHealth() - HealthDamage);
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		FString::Printf(TEXT("Remaining Tree Health: %f"), _Tree->GetHealth())
	);
	if (_Tree->GetHealth() <= 0)
	{
		const auto& _PlayerState = Cast<AGenericGaemPlayerState>(GetOwner());
		const auto& _NewMoney = FCString::Atof(*_PlayerState->GetMoney()) + _Tree->BreakTree();
		_PlayerState->SetMoney(FString::SanitizeFloat(_NewMoney));
	}
}
