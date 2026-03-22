// Copyright CityBuilder. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CBIsometricCameraDA.generated.h"

/**
 * Configuration data for the isometric camera system.
 * Create an instance in the editor and assign it to CBIsometricCameraPawn.
 */
UCLASS()
class CITYBUILDER_API UCBIsometricCameraDA : public UDataAsset
{
	GENERATED_BODY()

	// ===== UPROPERTY Variables =====
public:
	// --- Movement ---

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Movement")
	float MoveSpeed = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Movement")
	float EdgeScrollSpeed = 1000.0f;

	/** Pixels from screen edge to trigger edge scrolling */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Movement")
	float EdgeScrollThreshold = 20.0f;

	/** How fast WASD movement accelerates to full speed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Movement")
	float MoveAcceleration = 8.0f;

	/** How fast WASD movement decelerates to zero when input stops */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Movement")
	float MoveDeceleration = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Movement")
	float DragPanSpeed = 100.0f;

	// --- Zoom ---

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Zoom")
	float ZoomSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Zoom")
	float MinZoomDistance = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Zoom")
	float MaxZoomDistance = 8000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Zoom")
	float DefaultZoomDistance = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Zoom")
	float ZoomInterpSpeed = 6.0f;

	// --- Rotation ---

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Rotation")
	float RotationSpeed = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Rotation")
	float RotationInterpSpeed = 8.0f;

	// --- Tilt ---

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Tilt")
	float TiltSpeed = 2.0f;

	/** Negative values = looking down. -70 means steep top-down view */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Tilt")
	float MinPitchAngle = -70.0f;

	/** -20 means nearly horizontal view */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Tilt")
	float MaxPitchAngle = -20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Tilt")
	float DefaultPitchAngle = -45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|IsoCamData|Tilt")
	float TiltInterpSpeed = 6.0f;
};
