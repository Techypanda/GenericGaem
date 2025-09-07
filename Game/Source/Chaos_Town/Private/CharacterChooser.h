// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterChooser.generated.h"

UCLASS()
class ACharacterChooser : public APawn
{
	GENERATED_BODY()

public:
	ACharacterChooser();
protected:
	virtual void BeginPlay() override;
	/// <summary>
	/// The camera to look through to
	/// </summary>
	UPROPERTY(EditAnywhere)
	class UCameraComponent* _CameraActor;
	/// <summary>
	/// The class of the display actor spawnpoint to find on spawn
	/// </summary>
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ATargetPoint> _DisplayActorSPClass;
	/// <summary>
	/// Client to spawn a display actor at the spawn point, makes it so it doesn't exist on server
	/// </summary>
	void _SpawnDisplayActor(int Index);
	/// <summary>
	/// Moves character selector left or right by CharacterCount
	/// </summary>
	/// <param name="CharacterCount">negative to go left, positive to go right</param>
	UFUNCTION(BlueprintCallable)
	void _ChangeCharacter(int CharacterCount);
	/// <summary>
	/// Gets the current displayed character's name for display purposes
	/// </summary>
	UFUNCTION(BlueprintCallable)
	FName _GetCurrentCharacterName() const;
	/// <summary>
	/// Gets the current displayed character's price for display purposes
	/// </summary>
	UFUNCTION(BlueprintCallable)
	float _GetCurrentCharacterPrice();
	/// <summary>
	/// Returns the currently displayed character actor
	/// </summary>
	/// <returns>Actor</returns>
	UFUNCTION(BlueprintCallable)
	class AGameCharacter* GetCurrentCharacter() const;
private:
	class ATargetPoint* _DisplayActorSpawnPoint;
	class AGameCharacter* _SpawnedActor;
	int _CurrentIndex = 0;
	/// <summary>
	/// Helper method that gets character classes from gamestate
	/// </summary>
	TArray<TSubclassOf<class AGameCharacter>> _GetGameCharacterClasses();
};
