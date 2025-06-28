// Fill out your copyright notice in the Description page of Project Settings.


#include "ERole.h"
#include "BaseRole.h"
#include "LeaderRole.h"
#include "PeasantRole.h"

// Ugly way of converting ERole to IRole, im sure theres a nicer way using templating
std::unique_ptr<IRole> ERoleHelper::ERoleToRole(ERole InRole)
{
	switch (InRole)
	{
	case ERole::Leader:
		return std::make_unique<LeaderRole>();
	case ERole::Peasant:
		return std::make_unique<PeasantRole>();
	default:
		return std::make_unique<BaseRole>();
	}
}