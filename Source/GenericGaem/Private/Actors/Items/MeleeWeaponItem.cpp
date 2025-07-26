// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponItem.h"
#include "GenericGaemPlayerState.h"
#include "GenericGaemCharacterController.h"
#include "GenericGaemCharacter.h"
#include "GenericGaemMode.h"

void AMeleeWeaponItem::Use()
{
	Attack();
}

void AMeleeWeaponItem::Attack()
{
	// TODO: add delay
	const auto& _Character = GetCharacter();
	const auto& HitDetected = RaytraceForHit(_Character, _Character->GetActorForwardVector());
	if (!HitDetected)
	{
		return;
	}
	// Invoke Server to do the work
	ServerAttack(_Character->GetActorForwardVector());
}

AActor* AMeleeWeaponItem::RaytraceForHit(AGenericGaemCharacter* _Character, FVector ActorForwardVector, bool bDrawDebugLine) const
{
	ECollisionChannel TraceChannelProperty = ECC_Pawn;
	FHitResult Hit;
	FVector StartOfTrace = _Character->GetActorLocation();
	FVector EndOfTrace = StartOfTrace + ActorForwardVector * HitDistance;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(_Character);
	const auto _World = _Character->GetWorld();
	_World->LineTraceSingleByChannel(Hit, StartOfTrace, EndOfTrace, TraceChannelProperty, QueryParams);
	const auto& HitActor = Hit.GetActor();
	const auto& HitSucceededOnPlayer = Hit.bBlockingHit && IsValid(HitActor);
	if (bDrawDebugLine)
	{
		DrawDebugLine(_World, StartOfTrace, EndOfTrace, HitSucceededOnPlayer ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
	}
	if (!HitSucceededOnPlayer)
	{
		return nullptr;
	}
	return HitActor;
}

AGenericGaemCharacter* AMeleeWeaponItem::GetCharacter()
{
	const auto& _PlayerState = Cast<AGenericGaemPlayerState>(GetOwner());
	const auto& _Controller = Cast<APlayerController>(_PlayerState->GetOwner());
	const auto& _Character = Cast<AGenericGaemCharacter>(_Controller->GetCharacter());
	return _Character;
}

void AMeleeWeaponItem::ServerAttack_Implementation(FVector ActorForwardVector)
{
	UE_LOG(LogTemp, Warning, TEXT("Server Attack called on %s"), *GetName());
	const auto& _Character = GetCharacter();
	const auto _World = _Character->GetWorld();
	const auto& HitActor = RaytraceForHit(_Character, ActorForwardVector, true);
	if (!HitActor)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Server Attack called, hit actor: %s"), *HitActor->GetName());
	const auto _GameCharacter = Cast<AGenericGaemCharacter>(HitActor);
	if (!_GameCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit actor is not a GenericGaemCharacter: %s"), *HitActor->GetName());
		return;
	}
	const auto _GamePlayerState = _GameCharacter->GetPlayerState<AGenericGaemPlayerState>();
	if (!_GamePlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit actor's player state is not a GenericGaemPlayerState: %s"), *HitActor->GetName());
		return;
	}
	_GamePlayerState->SetHealth(_GamePlayerState->GetHealth() - HealthDamage);
}