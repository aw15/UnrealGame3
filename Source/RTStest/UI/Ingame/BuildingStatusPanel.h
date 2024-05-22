// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingStatusPanel.generated.h"

class UBuildingStatusUpgradeData;
class ABuildingSlot;
/**
 * 
 */
UCLASS()
class RTSTEST_API UBuildingStatusPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UBuildingStatusPanel(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Update(ABuildingSlot* BuildingSlot);
};
