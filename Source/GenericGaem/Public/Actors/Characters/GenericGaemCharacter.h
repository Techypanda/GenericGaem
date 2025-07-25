// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IRole.h"
#include "DeathObject.h"
#include "GenericGaemCharacter.generated.h"

UCLASS()
class GENERICGAEM_API AGenericGaemCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	bool bIsFirstPerson;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
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
	UFUNCTION(BlueprintCallable)
	void Death();
	void Revive();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TextDisplay);
	class UTextRenderComponent* RoleDisplayComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Role);
	class UDataTable* RoleTable;
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
	void SetShowStatusDisplays(bool bShow);
private:
	bool bIsHoldingRightClickInThirdPerson, bAllowZoom, bBindedTextRender;;
	UPROPERTY(Replicated)
	bool bDisableMovement;
	UPROPERTY(Replicated)
	bool bDisableLook;
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
	void SetVisibilityBasedOffRole(FString NewRole);
	void SetVulnerabilityBasedOffRole(FString NewRole);
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
