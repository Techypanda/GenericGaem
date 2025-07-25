// Fill out your copyright notice in the Description page of Project Settings.


#include "RoleName.h"

TMap<ERoleName, FString> ERoleHelper::ERoleNameToString = {
	{ERoleName::Peasant, "Peasant"},
	{ERoleName::Citizen, "Citizen"},
	{ERoleName::Monkey, "Monkey"},
	{ERoleName::Leader, "Leader"},
	{ERoleName::None, "None"}
};
TMap<FString, ERoleName> ERoleHelper::StringToERoleName = {
	{"Peasant", ERoleName::Peasant},
	{"Citizen", ERoleName::Citizen},
	{"Monkey", ERoleName::Monkey},
	{"Leader", ERoleName::Leader},
	{"None", ERoleName::None}
};