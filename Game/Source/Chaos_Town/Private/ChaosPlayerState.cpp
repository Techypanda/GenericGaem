#include "ChaosPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

AChaosPlayerState::AChaosPlayerState()
{
	bReplicates = true;
}

void AChaosPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AChaosPlayerState, _Money);
	DOREPLIFETIME(AChaosPlayerState, _RoleName);
}

float AChaosPlayerState::GetMoney() const
{
	return _Money;
}

void AChaosPlayerState::SetMoney(const float& NewMoney)
{
	if (GetRemoteRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Error, TEXT("SetMoney can only be called on the server"));
		return;
	}
	_Money = NewMoney;
	OnMoneyChanged();
}

FName AChaosPlayerState::GetRoleName() const
{
	return _RoleName;
}

void AChaosPlayerState::SetRoleName(const FName& NewRoleName)
{
	if (GetRemoteRole() != ROLE_Authority)
	{
		UE_LOG(LogTemp, Error, TEXT("SetRoleName can only be called on the server"));
		return;
	}
	_RoleName = NewRoleName;
	OnRoleChanged();
}

void AChaosPlayerState::OnRep_Money()
{
	OnMoneyChanged();
}

void AChaosPlayerState::OnMoneyChanged()
{
}

void AChaosPlayerState::OnRep_Role()
{
	OnRoleChanged();
}

void AChaosPlayerState::OnRoleChanged()
{
}
