// Fill out your copyright notice in the Description page of Project Settings.


#include "ERole.h"
#include "BaseRole.h"
#include "LeaderRole.h"
#include "PeasantRole.h"
#include "CitizenRole.h"
#include "MonkeyRole.h"

// Ugly way of converting ERole to IRole, im sure theres a nicer way using templating
// TODO: Swap from this approach to the TScriptable
IRole* ERoleHelper::ERoleToRole(ERole InRole)
{
	switch (InRole)
	{
	case ERole::Leader:
		return NewObject<ULeaderRole>();
	case ERole::Peasant:
		return NewObject<UPeasantRole>();
	case ERole::Citizen:
		return NewObject<UCitizenRole>();
	case ERole::Monkey:
		return NewObject<UMonkeyRole>();
	default:
		return NewObject<UBaseRole>();
	}
}

std::vector<IRole*> ERoleHelper::GetAllRoles()
{
	auto OutVector = std::vector<IRole*>{};
	for (int I = 0; I <= static_cast<int>(ERole::Leader); I++)
	{
		OutVector.push_back(ERoleHelper::ERoleToRole(static_cast<ERole>(I)));
	}
	return OutVector;
}
