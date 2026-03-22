// Copyright CityBuilder. All Rights Reserved.

#include "CB/Systems/InputSystem/CBInputControllerComp.h"
#include "CB/Utils/CBLogCategories.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "Framework/Application/SlateApplication.h"

UCBInputControllerComp::UCBInputControllerComp()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCBInputControllerComp::BeginPlay()
{
	Super::BeginPlay();
	InitializeInputMaps();
}

void UCBInputControllerComp::InitializeInputMaps()
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!PC)
	{
		UE_LOG(LogCBInputController, Warning, TEXT("Owner is not a PlayerController"));
		return;
	}
	
	if (bIsGameModeOnStart)
	{
		SetInputMaps(DefaultGameModeInputMaps, false);

		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());

		UE_LOG(LogCBInputController, Log, TEXT("Initializing %d input maps"), DefaultGameModeInputMaps.Num());
	}
	else
	{
		EnableUIMode();
		GetWorld()->GetTimerManager().SetTimerForNextTick([]() { FSlateApplication::Get().SetAllUserFocusToGameViewport(); });

		UE_LOG(LogCBInputController, Log, TEXT("Initializing %d input maps"), DefaultUIModeInputMaps.Num());
	}

}

void UCBInputControllerComp::SetInputMaps(const TArray<UInputMappingContext*>& pNewMaps, bool pWithClear, bool pWithFlush)
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!PC)
	{
		UE_LOG(LogCBInputController, Warning, TEXT("Owner is not a PlayerController"));
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		PrevInputMaps = CurrentInputMaps;

		if (pWithClear)
		{
			Subsystem->ClearAllMappings();
			CurrentInputMaps.Empty();
		}

		for (UInputMappingContext* NewContext : pNewMaps)
		{
			if (NewContext)
			{
				Subsystem->AddMappingContext(NewContext, 0);
				CurrentInputMaps.Add(NewContext);
			}
		}

		if (pWithFlush)
		{
			PC->FlushPressedKeys();
		}

		UE_LOG(LogCBInputController, Log, TEXT("Set %d input maps (WithClear: %s, WithFlush: %s)"),
			pNewMaps.Num(),
			pWithClear ? TEXT("true") : TEXT("false"),
			pWithFlush ? TEXT("true") : TEXT("false"));

		InputMapChanged.Broadcast(PrevInputMaps, CurrentInputMaps, this);
	}
	else
	{
		UE_LOG(LogCBInputController, Error, TEXT("Failed to get EnhancedInputLocalPlayerSubsystem"));
	}
}

void UCBInputControllerComp::EnableUIMode()
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!PC)
	{
		UE_LOG(LogCBInputController, Warning, TEXT("Owner is not a PlayerController"));
		return;
	}

	SetInputMaps(DefaultUIModeInputMaps, true);

	PC->bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	PC->SetInputMode(InputMode);

	UE_LOG(LogCBInputController, Log, TEXT("UI mode enabled with %d input maps"), DefaultUIModeInputMaps.Num());
}

void UCBInputControllerComp::DisableUIMode()
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!PC)
	{
		UE_LOG(LogCBInputController, Warning, TEXT("Owner is not a PlayerController"));
		return;
	}

	// Copy before SetInputMaps overwrites PrevInputMaps
	TArray<UInputMappingContext*> MapsToRestore = PrevInputMaps;
	SetInputMaps(MapsToRestore, true);

	PC->bShowMouseCursor = false;
	PC->SetInputMode(FInputModeGameOnly());

	UE_LOG(LogCBInputController, Log, TEXT("Restored %d input maps"), MapsToRestore.Num());
}

bool UCBInputControllerComp::IsUIMode() const
{
	if (DefaultUIModeInputMaps.IsEmpty())
	{
		return false;
	}

	for (const UInputMappingContext* Context : DefaultUIModeInputMaps)
	{
		if (!CurrentInputMaps.Contains(Context))
		{
			return false;
		}
	}

	return true;
}
