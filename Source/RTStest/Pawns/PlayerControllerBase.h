// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTStest/Common/PredefineMacro.h"
#include "PlayerControllerBase.generated.h"

class ASelectableUnit;
class AMovableUnit;
class APlayerBase;
class ABuildingSlot;
class ISetQuestTarget;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResetUiSelectionEvent);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnResetSlotSelction, AActor*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNeedToUpdateBuildingStatusPanel, ABuildingSlot*, InBuildingSlot);
UCLASS()
class RTSTEST_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
public:
	APlayerControllerBase();
	DeclareInstanceGetter(APlayerControllerBase)
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void EdgeScrolling() const;
	virtual void SetupInputComponent() override;
	void SetLastSelectedPlayerBuildingSlot(ABuildingSlot* BuildingSlotRef);
	AActor* GetLastSelectedSlot() const;
	ISetQuestTarget* GetLastSelectedMovableUnit() const;
	UFUNCTION(BlueprintCallable)
	void ClearLastSelectedBuildingSlot();
	void SetLastSelectedMovableUnit(AMovableUnit* InMovableUnitRef);
protected:
	UFUNCTION(BlueprintCallable)
	APlayerBase* GetPlayer() const;
	void Zoom(const float Amount);
	void MoveVertical(const float Amount);
	void MoveHorizontal(const float Amount);
	void LeftMousePressed();
	// void LeftMouseRelease();
	void RightMousePressed();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Component")
	class UBuildingUnitController* BuildingController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Component")
	class UMovableUnitSpawner* MovableUnitController;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FResetUiSelectionEvent OnResetUiSelectionEvent;
	
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnNeedToUpdateBuildingStatusPanel OnNeedToUpdateBuildingStatusPanel;

	FVector LastRightMouseHitLocation;
	FVector LastLeftMouseHitLocation;
	
protected:
    UPROPERTY(BlueprintReadOnly, Category="Unit")
	TWeakObjectPtr<AMovableUnit> LastSelectedMovableUnit;

private:
	TWeakObjectPtr<ABuildingSlot> LastSelectedPlayerBuildingSlot;
};
