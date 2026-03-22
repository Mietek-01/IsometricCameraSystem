// Copyright CityBuilder. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CBIsometricCameraPawn.generated.h"

class USceneComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UCBIsometricCameraDA;
struct FInputActionValue;

/**
 * Isometric camera pawn for the city builder.
 * Supports: orbiting, zoom, pan (WASD / edge scroll / middle mouse drag), tilt.
 */
UCLASS()
class CITYBUILDER_API ACBIsometricCameraPawn : public APawn
{
	GENERATED_BODY()

	// ===== Constructor, Virtual, Override =====
public:
	ACBIsometricCameraPawn();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// ===== UPROPERTY Variables =====
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CB|IsoCam|Refs")
	TObjectPtr<USceneComponent> RootPivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CB|IsoCam|Refs")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CB|IsoCam|Refs")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCam|Data")
	TObjectPtr<UCBIsometricCameraDA> CameraData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCam|Refs")
	TObjectPtr<UInputAction> CameraMoveIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCam|Refs")
	TObjectPtr<UInputAction> CameraZoomIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCam|Refs")
	TObjectPtr<UInputAction> CameraRotateIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCam|Refs")
	TObjectPtr<UInputAction> CameraTiltIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCam|Refs")
	TObjectPtr<UInputAction> CameraDragPanIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCam|Refs")
	TObjectPtr<UInputAction> CameraDragPanToggleIA;

	// ===== Internal Variables =====
private:
	float TargetZoomDistance = 0.0f;
	float TargetYawRotation = 0.0f;
	float TargetPitchAngle = 0.0f;

	bool bIsDragPanning = false;
	bool bIsFirstDragPanFrame = false;

	FVector2D PendingMoveInput = FVector2D::ZeroVector;
	FVector2D CurrentMoveVelocity = FVector2D::ZeroVector;

	// ===== Internal Methods =====
private:
	void HandleMove(const FInputActionValue& pValue);
	void HandleZoom(const FInputActionValue& pValue);
	void HandleRotate(const FInputActionValue& pValue);
	void HandleTilt(const FInputActionValue& pValue);
	void HandleDragPan(const FInputActionValue& pValue);
	void HandleDragPanStarted(const FInputActionValue& pValue);
	void HandleDragPanCompleted(const FInputActionValue& pValue);

	void ProcessEdgeScrolling(float pDeltaTime);
	void ApplyMovement(const FVector2D& pDirection, float pSpeed, float pDeltaTime);
	void UpdateSmoothZoom(float pDeltaTime);
	void UpdateSmoothRotation(float pDeltaTime);
	void UpdateSmoothTilt(float pDeltaTime);
};
