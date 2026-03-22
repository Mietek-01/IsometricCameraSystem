// Copyright CityBuilder. All Rights Reserved.

#include "CB/Systems/IsometricCameraSystem/CBIsometricCameraPawn.h"
#include "CB/Systems/IsometricCameraSystem/Data/CBIsometricCameraDA.h"
#include "CB/Utils/CBLogCategories.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"

ACBIsometricCameraPawn::ACBIsometricCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	RootPivot = CreateDefaultSubobject<USceneComponent>(TEXT("RootPivot"));
	SetRootComponent(RootPivot);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootPivot);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bUsePawnControlRotation = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ACBIsometricCameraPawn::BeginPlay()
{
	Super::BeginPlay();

	if (!CameraData)
	{
		UE_LOG(LogCBIsometricCameraSystem, Error, TEXT("BeginPlay: CameraData is null! Assign a UCBIsometricCameraDA."));
		SetActorTickEnabled(false);
		return;
	}

	SetActorTickEnabled(true);

	TargetZoomDistance = CameraData->DefaultZoomDistance;
	TargetPitchAngle = CameraData->DefaultPitchAngle;
	TargetYawRotation = 0.0f;

	SpringArm->TargetArmLength = TargetZoomDistance;
	SpringArm->SetRelativeRotation(FRotator(TargetPitchAngle, TargetYawRotation, 0.0f));

	UE_LOG(LogCBIsometricCameraSystem, Log, TEXT("BeginPlay: Camera initialized (Zoom: %.0f, Pitch: %.1f)"),
		TargetZoomDistance, TargetPitchAngle);
}

void ACBIsometricCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ProcessEdgeScrolling(DeltaTime);

	// Smooth WASD movement: accelerate toward input, decelerate when released
	if (!PendingMoveInput.IsNearlyZero())
	{
		CurrentMoveVelocity = FMath::Vector2DInterpTo(CurrentMoveVelocity, PendingMoveInput, DeltaTime, CameraData->MoveAcceleration);
		PendingMoveInput = FVector2D::ZeroVector;
	}
	else
	{
		CurrentMoveVelocity = FMath::Vector2DInterpTo(CurrentMoveVelocity, FVector2D::ZeroVector, DeltaTime, CameraData->MoveDeceleration);
	}

	if (!CurrentMoveVelocity.IsNearlyZero(0.01f))
	{
		ApplyMovement(CurrentMoveVelocity, CameraData->MoveSpeed, DeltaTime);
	}

	UpdateSmoothZoom(DeltaTime);
	UpdateSmoothRotation(DeltaTime);
	UpdateSmoothTilt(DeltaTime);
}

void ACBIsometricCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput)
	{
		UE_LOG(LogCBIsometricCameraSystem, Error, TEXT("SetupPlayerInputComponent: PlayerInputComponent is not UEnhancedInputComponent!"));
		return;
	}

	if (CameraMoveIA)
	{
		EnhancedInput->BindAction(CameraMoveIA, ETriggerEvent::Triggered, this, &ACBIsometricCameraPawn::HandleMove);
	}

	if (CameraZoomIA)
	{
		EnhancedInput->BindAction(CameraZoomIA, ETriggerEvent::Triggered, this, &ACBIsometricCameraPawn::HandleZoom);
	}

	if (CameraRotateIA)
	{
		EnhancedInput->BindAction(CameraRotateIA, ETriggerEvent::Triggered, this, &ACBIsometricCameraPawn::HandleRotate);
	}

	if (CameraTiltIA)
	{
		EnhancedInput->BindAction(CameraTiltIA, ETriggerEvent::Triggered, this, &ACBIsometricCameraPawn::HandleTilt);
	}

	if (CameraDragPanIA)
	{
		EnhancedInput->BindAction(CameraDragPanIA, ETriggerEvent::Triggered, this, &ACBIsometricCameraPawn::HandleDragPan);
	}

	if (CameraDragPanToggleIA)
	{
		EnhancedInput->BindAction(CameraDragPanToggleIA, ETriggerEvent::Started, this, &ACBIsometricCameraPawn::HandleDragPanStarted);
		EnhancedInput->BindAction(CameraDragPanToggleIA, ETriggerEvent::Completed, this, &ACBIsometricCameraPawn::HandleDragPanCompleted);
	}

	UE_LOG(LogCBIsometricCameraSystem, Log, TEXT("SetupPlayerInputComponent: Input actions bound."));
}

// --- Input Handlers ---

void ACBIsometricCameraPawn::HandleMove(const FInputActionValue& pValue)
{
	PendingMoveInput = pValue.Get<FVector2D>();
}

void ACBIsometricCameraPawn::HandleZoom(const FInputActionValue& pValue)
{
	if (!CameraData)
	{
		return;
	}

	const float ZoomDelta = pValue.Get<float>();
	TargetZoomDistance = FMath::Clamp(
		TargetZoomDistance - ZoomDelta * CameraData->ZoomSpeed,
		CameraData->MinZoomDistance,
		CameraData->MaxZoomDistance
	);
}

void ACBIsometricCameraPawn::HandleRotate(const FInputActionValue& pValue)
{
	if (!CameraData)
	{
		return;
	}

	const float RotateDelta = pValue.Get<float>();
	TargetYawRotation = FRotator::NormalizeAxis(TargetYawRotation + RotateDelta * CameraData->RotationSpeed);
}

void ACBIsometricCameraPawn::HandleTilt(const FInputActionValue& pValue)
{
	if (!CameraData)
	{
		return;
	}

	const float TiltDelta = pValue.Get<float>();
	TargetPitchAngle = FMath::Clamp(
		TargetPitchAngle + TiltDelta * CameraData->TiltSpeed,
		CameraData->MinPitchAngle,
		CameraData->MaxPitchAngle
	);
}

void ACBIsometricCameraPawn::HandleDragPan(const FInputActionValue& pValue)
{
	if (!bIsDragPanning || !CameraData)
	{
		return;
	}

	if (bIsFirstDragPanFrame)
	{
		bIsFirstDragPanFrame = false;
		return;
	}

	const FVector2D Delta = pValue.Get<FVector2D>();
	if (Delta.IsNearlyZero())
	{
		return;
	}

	const float CameraYaw = SpringArm->GetRelativeRotation().Yaw;
	const FRotator YawRotation(0.0f, CameraYaw, 0.0f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	const FVector Movement = (Forward * -Delta.Y + Right * -Delta.X) * CameraData->DragPanSpeed;
	RootPivot->AddWorldOffset(Movement);
}

void ACBIsometricCameraPawn::HandleDragPanStarted(const FInputActionValue& pValue)
{
	bIsDragPanning = true;
	bIsFirstDragPanFrame = true;

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->bShowMouseCursor = false;
	}
}

void ACBIsometricCameraPawn::HandleDragPanCompleted(const FInputActionValue& pValue)
{
	bIsDragPanning = false;

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->bShowMouseCursor = true;
	}
}

// --- Internal Logic ---

void ACBIsometricCameraPawn::ProcessEdgeScrolling(float pDeltaTime)
{
	if (bIsDragPanning)
	{
		return;
	}

	const APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return;
	}

	float MouseX = 0.0f;
	float MouseY = 0.0f;
	if (!PC->GetMousePosition(MouseX, MouseY))
	{
		return;
	}

	int32 ViewportSizeX = 0;
	int32 ViewportSizeY = 0;
	PC->GetViewportSize(ViewportSizeX, ViewportSizeY);

	if (ViewportSizeX == 0 || ViewportSizeY == 0)
	{
		return;
	}

	const float Threshold = CameraData->EdgeScrollThreshold;
	FVector2D ScrollDirection = FVector2D::ZeroVector;

	if (MouseX <= Threshold)
	{
		ScrollDirection.X = -1.0f;
	}
	else if (MouseX >= ViewportSizeX - Threshold)
	{
		ScrollDirection.X = 1.0f;
	}

	if (MouseY <= Threshold)
	{
		ScrollDirection.Y = 1.0f;
	}
	else if (MouseY >= ViewportSizeY - Threshold)
	{
		ScrollDirection.Y = -1.0f;
	}

	if (!ScrollDirection.IsNearlyZero())
	{
		ApplyMovement(ScrollDirection, CameraData->EdgeScrollSpeed, pDeltaTime);
	}
}

void ACBIsometricCameraPawn::ApplyMovement(const FVector2D& pDirection, float pSpeed, float pDeltaTime)
{
	if (pDirection.IsNearlyZero())
	{
		return;
	}

	const float CameraYaw = SpringArm->GetRelativeRotation().Yaw;
	const FRotator YawRotation(0.0f, CameraYaw, 0.0f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	const FVector Movement = (Forward * pDirection.Y + Right * pDirection.X) * pSpeed * pDeltaTime;
	RootPivot->AddWorldOffset(Movement);
}

void ACBIsometricCameraPawn::UpdateSmoothZoom(float pDeltaTime)
{
	const float CurrentZoom = SpringArm->TargetArmLength;
	if (FMath::IsNearlyEqual(CurrentZoom, TargetZoomDistance, 0.1f))
	{
		return;
	}

	SpringArm->TargetArmLength = FMath::FInterpTo(CurrentZoom, TargetZoomDistance, pDeltaTime, CameraData->ZoomInterpSpeed);
}

void ACBIsometricCameraPawn::UpdateSmoothRotation(float pDeltaTime)
{
	FRotator CurrentRotation = SpringArm->GetRelativeRotation();
	const float DeltaAngle = FRotator::NormalizeAxis(TargetYawRotation - CurrentRotation.Yaw);

	if (FMath::IsNearlyZero(DeltaAngle, 0.05f))
	{
		return;
	}

	const float NewYaw = CurrentRotation.Yaw + DeltaAngle * FMath::Clamp(pDeltaTime * CameraData->RotationInterpSpeed, 0.0f, 1.0f);
	SpringArm->SetRelativeRotation(FRotator(CurrentRotation.Pitch, NewYaw, 0.0f));
}

void ACBIsometricCameraPawn::UpdateSmoothTilt(float pDeltaTime)
{
	FRotator CurrentRotation = SpringArm->GetRelativeRotation();
	if (FMath::IsNearlyEqual(CurrentRotation.Pitch, TargetPitchAngle, 0.05f))
	{
		return;
	}

	const float NewPitch = FMath::FInterpTo(CurrentRotation.Pitch, TargetPitchAngle, pDeltaTime, CameraData->TiltInterpSpeed);
	SpringArm->SetRelativeRotation(FRotator(NewPitch, CurrentRotation.Yaw, 0.0f));
}
