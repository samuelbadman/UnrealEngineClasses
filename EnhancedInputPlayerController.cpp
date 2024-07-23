// Fill out your copyright notice in the Description page of Project Settings.

#include "EnhancedInputPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/Commands/InputChord.h"

void AEnhancedInputPlayerController::AddInputMappingContext(TSoftObjectPtr<UInputMappingContext> InputMappingContext, const int32 Priority)
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext.IsNull())
			{
				return;
			}

			UInputMappingContext* InputMappingContextRef = InputMappingContext.LoadSynchronous();

			if (IsValid(InputMappingContextRef))
			{
				InputSystem->AddMappingContext(InputMappingContextRef, Priority);
			}
		}
	}
}

void AEnhancedInputPlayerController::AddInputMappingContext(UInputMappingContext* InputMappingContext, const int32 Priority)
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IsValid(InputMappingContext))
			{
				InputSystem->AddMappingContext(InputMappingContext, Priority);
			}
		}
	}
}

void AEnhancedInputPlayerController::RemoveInputMappingContext(UInputMappingContext* InputMappingContext)
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSystem->RemoveMappingContext(InputMappingContext);
		}
	}
}

void AEnhancedInputPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	BindInput();
}

void AEnhancedInputPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	DetectMouseMove();
}

void AEnhancedInputPlayerController::BindEnhancedInputActions(UEnhancedInputComponent* const EnhancedInputComponent)
{
}

void AEnhancedInputPlayerController::OnAnyKeyPressed(const FKey Key)
{
	UpdateUsingGamepad(Key);
}

void AEnhancedInputPlayerController::OnAnyKeyReleased(const FKey Key)
{
	UpdateUsingGamepad(Key);
}

void AEnhancedInputPlayerController::OnMouseMove(const FVector2D& Delta)
{
	UpdateUsingGamepad(EKeys::Mouse2D);
}

void AEnhancedInputPlayerController::BindAnyKeyInput()
{
	FInputKeyBinding AnyKeyPressedBinding(FInputChord(EKeys::AnyKey), EInputEvent::IE_Pressed);
	AnyKeyPressedBinding.bConsumeInput = false;
	AnyKeyPressedBinding.bExecuteWhenPaused = true;
	AnyKeyPressedBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindUObject(this, &AEnhancedInputPlayerController::OnAnyKeyPressed);

	InputComponent->KeyBindings.Add(AnyKeyPressedBinding);

	FInputKeyBinding AnyKeyReleasedBinding(FInputChord(EKeys::AnyKey), EInputEvent::IE_Released);
	AnyKeyReleasedBinding.bConsumeInput = false;
	AnyKeyReleasedBinding.bExecuteWhenPaused = true;
	AnyKeyReleasedBinding.KeyDelegate.GetDelegateWithKeyForManualSet().BindUObject(this, &AEnhancedInputPlayerController::OnAnyKeyReleased);

	InputComponent->KeyBindings.Add(AnyKeyReleasedBinding);
}

void AEnhancedInputPlayerController::BindInput()
{
	BindAnyKeyInput();
	if (UEnhancedInputComponent* const EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		BindEnhancedInputActions(EnhancedInputComponent);
	}
}

bool AEnhancedInputPlayerController::DetectMouseMove()
{
	FVector2D CurrentMousePosition = {};

	// GetMousePosition returns false if there is no associated mouse device.
	if (!GetMousePosition(CurrentMousePosition.X, CurrentMousePosition.Y))
	{
		return false;
	}

	if (CurrentMousePosition != PreviousMousePosition)
	{
		// Mouse position has changed.
		OnMouseMove(CurrentMousePosition - PreviousMousePosition);
		PreviousMousePosition = CurrentMousePosition;
		return true;
	}

	// Mouse position has not changed.
	return false;
}

void AEnhancedInputPlayerController::UpdateUsingGamepad(const FKey Key)
{
	const bool bNewUsingGamepad = Key.IsGamepadKey();

	if (bNewUsingGamepad != bUsingGamepad)
	{
		bUsingGamepad = bNewUsingGamepad;
		InputDeviceChangedDelegate.Broadcast(bNewUsingGamepad);
	}
}
