// Fill out your copyright notice in the Description page of Project Settings.


#include "PeasantRole.h"

const std::string_view PeasantRole::RoleName = "Peasant";

PeasantRole::PeasantRole() : BaseRole(PeasantRole::RoleName)
{
}