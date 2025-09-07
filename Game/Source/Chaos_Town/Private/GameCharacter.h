// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameCharacter.generated.h"

UCLASS()
class AGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/// <summary>
	/// Returns the name of this character for display purposes
	/// </summary>
	/// <returns>Name of character</returns>
	UFUNCTION(BlueprintCallable)
	FName GetCharacterName() const { return _CharacterName; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/// <summary>
	/// Characters name for display purposes
	/// </summary>
	UPROPERTY(EditDefaultsOnly)
	FName _CharacterName;
};
