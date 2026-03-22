// Copyright CityBuilder. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBInputControllerComp.generated.h"

class UInputMappingContext;
class UCBInputControllerComp;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCBOnInputMapChanged,
	const TArray<UInputMappingContext*>&, PreviousMaps,
	const TArray<UInputMappingContext*>&, NewMaps,
	UCBInputControllerComp*, InputController);

/**
 * Component for managing input mapping contexts and UI/gameplay modes
 * Handles switching between gameplay input and UI input, including cursor visibility
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CITYBUILDER_API UCBInputControllerComp : public UActorComponent
{
	GENERATED_BODY()

	// ===== Constructor, Virtual, Override =====
public:
	UCBInputControllerComp();

protected:
	virtual void BeginPlay() override;

	// ===== Events =====
public:
	UPROPERTY(BlueprintAssignable, Category = "CB|InputController|Events")
	FCBOnInputMapChanged InputMapChanged;

	// ===== UPROPERTY Variables =====
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|InputController|Data")
	bool bIsGameModeOnStart = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|InputController|Data")
	TArray<UInputMappingContext*> DefaultGameModeInputMaps;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CB|InputController|Data")
	TArray<UInputMappingContext*> DefaultUIModeInputMaps;

private:
	UPROPERTY(VisibleAnywhere, Category = "CB|InputController|Runtime Data")
	TArray<UInputMappingContext*> CurrentInputMaps;

	UPROPERTY(VisibleAnywhere, Category = "CB|InputController|Runtime Data")
	TArray<UInputMappingContext*> PrevInputMaps;

	// ===== UFUNCTION Methods =====
public:
	/**
	 * @param pWithClear If true, clears current maps before adding new ones.
	 * @param pWithFlush If true, flushes all pressed keys after changing maps.
	 */
	UFUNCTION(BlueprintCallable, Category = "CB|InputController|Methods")
	void SetInputMaps(const TArray<UInputMappingContext*>& pNewMaps, bool pWithClear = true, bool pWithFlush = false);

	UFUNCTION(BlueprintPure, Category = "CB|InputController|Methods")
	bool IsUIMode() const;

	UFUNCTION(BlueprintCallable, Category = "CB|InputController|Methods")
	void EnableUIMode();

	UFUNCTION(BlueprintCallable, Category = "CB|InputController|Methods")
	void DisableUIMode();

	UFUNCTION(BlueprintPure, Category = "CB|InputController|Getters")
	const TArray<UInputMappingContext*>& GetCurrentInputMaps() const { return CurrentInputMaps; }

	UFUNCTION(BlueprintPure, Category = "CB|InputController|Getters")
	const TArray<UInputMappingContext*>& GetPrevInputMaps() const { return PrevInputMaps; }

	// ===== Internal Methods =====
private:
	void InitializeInputMaps();
};
