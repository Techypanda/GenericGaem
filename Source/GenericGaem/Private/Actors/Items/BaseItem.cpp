// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"
#include "Net/UnrealNetwork.h"

ABaseItem::ABaseItem()
{
	bReplicates = true;
	bNetUseOwnerRelevancy = true; // Use the owner's relevancy for this item
}

FString ABaseItem::Name()
{
	return ItemName;
}

void ABaseItem::Use()
{
}

void ABaseItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Add properties to replicate here if needed
	// DOREPLIFETIME(UBaseItem, PropertyName);
}
