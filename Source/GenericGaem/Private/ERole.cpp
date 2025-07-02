// Fill out your copyright notice in the Description page of Project Settings.


#include "ERole.h"
#include "BaseRole.h"
#include "LeaderRole.h"
#include "PeasantRole.h"
#include "CitizenRole.h"

// Ugly way of converting ERole to IRole, im sure theres a nicer way using templating
std::unique_ptr<IRole> ERoleHelper::ERoleToRole(ERole InRole)
{
	switch (InRole)
	{
	case ERole::Leader:
		return std::make_unique<LeaderRole>();
	case ERole::Peasant:
		return std::make_unique<PeasantRole>();
	case ERole::Citizen:
		return std::make_unique<CitizenRole>();
	default:
		return std::make_unique<BaseRole>();
	}
}

std::vector<std::unique_ptr<IRole>> ERoleHelper::GetAllRoles()
{
	auto OutVector = std::vector<std::unique_ptr<IRole>>{};
	for (int I = 0; I <= static_cast<int>(ERole::Leader); I++)
	{
		OutVector.push_back(ERoleHelper::ERoleToRole(static_cast<ERole>(I)));
	}
	return OutVector;
}
