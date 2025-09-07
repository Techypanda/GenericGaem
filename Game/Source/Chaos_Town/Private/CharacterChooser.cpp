#include "CharacterChooser.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameCharacter.h"
#include "ChaosGameState.h"
#include "Engine/TargetPoint.h"

ACharacterChooser::ACharacterChooser()
{
	PrimaryActorTick.bCanEverTick = true;
	_CameraActor = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraActor"));
	SetRootComponent(_CameraActor);
}

void ACharacterChooser::BeginPlay()
{
	Super::BeginPlay();
	_CurrentIndex = 0;
	_DisplayActorSpawnPoint = Cast<ATargetPoint>(UGameplayStatics::GetActorOfClass(GetWorld(), _DisplayActorSPClass));
	if (!_DisplayActorSpawnPoint) return;
	UE_LOG(LogTemp, Warning, TEXT("Found Display Actor Spawn Point: %s"), *_DisplayActorSpawnPoint->GetName());
	_SpawnDisplayActor(_CurrentIndex);
}

void ACharacterChooser::_SpawnDisplayActor(int Index)
{
	if (!IsLocallyControlled()) return;
	if (_SpawnedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying old spawned actor: %s"), *_SpawnedActor->GetName());
		_SpawnedActor->Destroy();
		_SpawnedActor = nullptr;
	}
	const auto& _DisplayActors = _GetGameCharacterClasses();
	UE_LOG(LogTemp, Warning, TEXT("Found %d display actors"), _DisplayActors.Num());
	if (_DisplayActors.IsValidIndex(Index))
	{
		const FTransform SpawnTransform = _DisplayActorSpawnPoint->GetActorTransform();
		FActorSpawnParameters SpawnParams;
		_SpawnedActor = GetWorld()->SpawnActor<AGameCharacter>(_DisplayActors[Index], SpawnTransform, SpawnParams);
		_SpawnedActor->SetReplicates(false);
		_SpawnedActor->bOnlyRelevantToOwner = true;
	}
}

void ACharacterChooser::_ChangeCharacter(int CharacterCount)
{
	const auto& _DisplayActors = _GetGameCharacterClasses();
	_CurrentIndex += CharacterCount;
	if (_CurrentIndex < 0)
	{
		_CurrentIndex = _DisplayActors.Num() - 1;
	}
	else if (_CurrentIndex >= _DisplayActors.Num())
	{
		_CurrentIndex = 0;
	}
	_SpawnDisplayActor(_CurrentIndex);
}

FName ACharacterChooser::_GetCurrentCharacterName() const
{
	if (_SpawnedActor && _SpawnedActor->GetCharacterName().IsValid())
	{
		return _SpawnedActor->GetCharacterName();
	}
	return FName("Unknown");
}

float ACharacterChooser::_GetCurrentCharacterPrice()
{
	if (_SpawnedActor && _SpawnedActor->GetCharacterName().IsValid())
	{
		return GetWorld()->GetGameState<AChaosGameState>()->GetPriceOfRoleCharacter(_SpawnedActor->GetCharacterName());
	}
	return 0.0f;
}

AGameCharacter* ACharacterChooser::GetCurrentCharacter() const
{
	return _SpawnedActor;
}

TArray<TSubclassOf<class AGameCharacter>> ACharacterChooser::_GetGameCharacterClasses()
{
	return GetWorld()->GetGameState<AChaosGameState>()->GetRoleCharacterClasses();
}
