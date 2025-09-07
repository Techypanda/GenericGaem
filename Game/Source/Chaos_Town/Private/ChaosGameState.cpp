#include "ChaosGameState.h"
#include "GameCharacter.h"
#include "RoleData.h"
#include "ChaosPlayerState.h"

TArray<TSubclassOf<AGameCharacter>> AChaosGameState::GetRoleCharacterClasses()
{
	_CacheRoleDataPricesIfNeeded();
	return _CachedCharacterClasses;
}

bool AChaosGameState::RoleExists(const FName& RoleName) const
{
	return _CachedCharacterPrices.Contains(RoleName);
}

TSubclassOf<AGameCharacter> AChaosGameState::GetRoleForName(const FName& RoleName) const
{
	return _RoleNameToCharacterClassMap[RoleName];
}

float AChaosGameState::GetPriceOfRoleCharacter(FName RoleName)
{
	_CacheRoleDataPricesIfNeeded();
	if (_CachedCharacterPrices.Contains(RoleName))
	{
		return _CachedCharacterPrices[RoleName];
	}
	return 0.0f;
}

void AChaosGameState::BeginPlay()
{
	Super::BeginPlay();
	_CacheRoleDataPricesIfNeeded();
}

bool AChaosGameState::PlayerCanPurchase(APlayerController* PlayerController, float Price)
{
	if (!PlayerController)
	{
		return false;
	}
	const AChaosPlayerState* _PlayerState = PlayerController->GetPlayerState<AChaosPlayerState>();
	if (!_PlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState is null, cannot purchase role"));
		return false;
	}
	return _PlayerState->GetMoney() >= Price;
}

void AChaosGameState::_LoadAndCacheRoleData()
{
	TArray<FRoleData*> Rows;
	_RoleTable->GetAllRows<FRoleData>(TEXT("GetRoleCharacterClasses"), Rows);
	TArray<FName> RowNames = _RoleTable->GetRowNames();
	_CacheRoleDataCharacterClasses(Rows);
	_CacheRoleDataPrices(RowNames);
}

void AChaosGameState::_CacheRoleDataCharacterClasses(TArray<struct FRoleData*>& RoleDataArr)
{
	for (const auto& _Role : RoleDataArr)
	{
		_CachedCharacterClasses.Add(_Role->RoleCharacter);
	}
}

void AChaosGameState::_CacheRoleDataPrices(TArray<FName>& RoleDataArr)
{
	for (const FName& _RoleName : RoleDataArr)
	{
		_CachedCharacterPrices.Add(_RoleName, _RoleTable->FindRow<FRoleData>(_RoleName, TEXT("_CacheRoleDataPrices"))->Price);
		_RoleNameToCharacterClassMap.Add(_RoleName, _RoleTable->FindRow<FRoleData>(_RoleName, TEXT("_CacheRoleDataPrices"))->RoleCharacter);
	}
}

void AChaosGameState::_CacheRoleDataPricesIfNeeded()
{
	if (_CachedCharacterPrices.Num() == 0 || _CachedCharacterClasses.Num() == 0)
	{
		_LoadAndCacheRoleData();
	}
}
