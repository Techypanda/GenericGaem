// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IRole.h"
#include "DeathObject.h"
#include "ERole.h"
#include "GenericGaemCharacter.generated.h"

UCLASS()
class GENERICGAEM_API AGenericGaemCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	bool bIsFirstPerson;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	AGenericGaemCharacter();
	void SetFirstPerson();
	void SetThirdPerson();
	class USpringArmComponent* GetCameraSpringArmComponent();
	const float MinimumZoom() const;
	const float MaximumZoom() const;
	const float ZoomMagnitude() const;
	UFUNCTION(BlueprintCallable)
	void EnableMovement(bool bEnabled);
	UFUNCTION(BlueprintCallable)
	void ShowCursor(bool bShowCursor);
	void OnRep_PlayerState() override;
	DECLARE_EVENT(FLayerViewModel, FEscapeMenuEvent)
	FEscapeMenuEvent& OnEscapeMenu() { return _EscapeMenuEvent; }
	UFUNCTION(Server, Reliable)
	void ServerDeath();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Death);
	class TSubclassOf<ADeathObject> DeathObject;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TextDisplay);
	class UTextRenderComponent* RoleDisplayComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TextDisplay);
	class UTextRenderComponent* HealthDisplayComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera);
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera);
	class USpringArmComponent* CameraSpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera);
	float MinimumZoomValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera);
	float MaximumZoomValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera);
	float ZoomMagnitudeValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TSoftObjectPtr<class UGenericGaemInputMapping> InputMapping;
	virtual void BeginPlay() override;
	virtual void OnPlayerStateChanged(APlayerState* NewPlayerState, APlayerState* OldPlayerState) override;
private:
	bool bIsHoldingRightClickInThirdPerson, bAllowZoom, bDisableMovement, bDisableLook, bBindedTextRender;
	float RcMouseX, RcMouseY;
	FEscapeMenuEvent _EscapeMenuEvent;
	class UTextRenderComponent* CreateTextRenderComponent(const wchar_t* const Name, const float& ZOffset, const float& FontSize);
	void Zoom(const struct FInputActionInstance& Instance);
	void EscapeMenu(const struct FInputActionInstance& Instance);
	void Look2D(const struct FInputActionInstance& Instance);
	void MoveForward(const struct FInputActionInstance& Instance);
	void MoveRight(const struct FInputActionInstance& Instance);
	void ThirdPersonRightClick(const struct FInputActionInstance& Instance);
	void Jump(const struct FInputActionInstance& Instance);
	void OnRoleChange();
	void OnHealthChange();
	void BindTextRenders();
	void Swim(const FInputActionInstance& Instance);
	void UseAction(const FInputActionInstance& Instance);
	void PossessedBy(AController* NewController) override;
	void SetVisibilityBasedOffRole(ERole NewRole);
	void SetVulnerabilityBasedOffRole(ERole NewRole);
	// Ugly to cleanup
	void SelectItem(int Idx);
	void SelectItem1(const FInputActionInstance& Instance);
	void SelectItem2(const FInputActionInstance& Instance);
	void SelectItem3(const FInputActionInstance& Instance);
	void SelectItem4(const FInputActionInstance& Instance);
	void SelectItem5(const FInputActionInstance& Instance);
	void SelectItem6(const FInputActionInstance& Instance);
	void SelectItem7(const FInputActionInstance& Instance);
	void SelectItem8(const FInputActionInstance& Instance);
	void SelectItem9(const FInputActionInstance& Instance);
};
