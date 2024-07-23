// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputPlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInputDeviceChangedSignature, bool /* bUsingGamepad */)

/**
 * 
 */
UCLASS()
class AEnhancedInputPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	FInputDeviceChangedSignature InputDeviceChangedDelegate = {};

private:
	FVector2D PreviousMousePosition = FVector2D::ZeroVector;
	bool bUsingGamepad = true;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsUsingGamepad() const { return bUsingGamepad; }

protected:
	void PlayerTick(float DeltaTime) override;

	void AddInputMappingContext(TSoftObjectPtr<UInputMappingContext> InputMappingContext, const int32 Priority);
	void AddInputMappingContext(UInputMappingContext* InputMappingContext, const int32 Priority);
	void RemoveInputMappingContext(UInputMappingContext* InputMappingContext);

private:
	void SetupInputComponent() override;

	// Enhanced input component pointer is safe to use.
	virtual void BindEnhancedInputActions(UEnhancedInputComponent* const EnhancedInputComponent);

	UFUNCTION()
	virtual void OnAnyKeyPressed(const FKey Key);
	UFUNCTION()
	virtual void OnAnyKeyReleased(const FKey Key);
	virtual void OnMouseMove(const FVector2D& Delta);

	void BindAnyKeyInput();
	void BindInput();
	bool DetectMouseMove();
	void UpdateUsingGamepad(const FKey Key);
};
